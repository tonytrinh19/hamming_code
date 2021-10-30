#include "ascii2hamming.h"

int main(int argc, char *argv[]) {
    dc_posix_tracer tracer;
    dc_error_reporter reporter;
    struct dc_posix_env env;
    struct dc_error err;
    struct dc_application_info *info;
    int ret_val;

    reporter = error_reporter;
    tracer = trace_reporter;
    tracer = NULL;
    dc_error_init(&err, reporter);
    dc_posix_env_init(&env, tracer);
    info = dc_application_info_create(&env, &err, "Ascii to hamming application");
    ret_val = dc_application_run(&env, &err, info, create_settings, destroy_settings, run, dc_default_create_lifecycle,
                                 dc_default_destroy_lifecycle, NULL, argc, argv);
    dc_application_info_destroy(&env, &info);
    dc_error_reset(&err);

    return ret_val;
}

static struct dc_application_settings *create_settings(const struct dc_posix_env *env, struct dc_error *err) {
    struct application_settings *settings;

    DC_TRACE(env);
    settings = dc_malloc(env, err, sizeof(struct application_settings));

    if (settings == NULL) {
        return NULL;
    }

    settings->opts.parent.config_path = dc_setting_path_create(env, err);
    settings->parity = dc_setting_string_create(env, err);
    settings->prefix = dc_setting_string_create(env, err);

    struct options opts[] = {
            {(struct dc_setting *) settings->opts.parent.config_path,
                    dc_options_set_path,
                    "config",
                    required_argument,
                    'c',
                    "CONFIG",
                    dc_string_from_string,
                    NULL,
                    dc_string_from_config,
                    NULL},
            {(struct dc_setting *) settings->parity,
                    dc_options_set_string,
                    "parity",
                    required_argument,
                    'p',
                    "PARITY",
                    dc_string_from_string,
                    "parity",
                    dc_string_from_config,
                    "even"},
            {(struct dc_setting *) settings->prefix,
                    dc_options_set_string,
                    "prefix",
                    required_argument,
                    'e',
                    "PREFIX",
                    dc_string_from_string,
                    "prefix",
                    dc_string_from_config,
                    "file"},
    };

    settings->opts.opts_count = (sizeof(opts) / sizeof(struct options)) + 1;
    settings->opts.opts_size = sizeof(struct options);
    settings->opts.opts = dc_calloc(env, err, settings->opts.opts_count, settings->opts.opts_size);
    dc_memcpy(env, settings->opts.opts, opts, sizeof(opts));
    settings->opts.flags = "p:e:";
    settings->opts.env_prefix = "ASCII_HAMMING_";

    return (struct dc_application_settings *) settings;
}

static int destroy_settings(const struct dc_posix_env *env,
                            __attribute__((unused)) struct dc_error *err,
                            struct dc_application_settings **psettings) {
    struct application_settings *app_settings;

    DC_TRACE(env);
    app_settings = (struct application_settings *) *psettings;
    dc_setting_string_destroy(env, &app_settings->parity);
    dc_setting_string_destroy(env, &app_settings->prefix);
    dc_free(env, app_settings->opts.opts, app_settings->opts.opts_count);
    dc_free(env, *psettings, sizeof(struct application_settings));

    if (env->null_free) {
        *psettings = NULL;
    }

    return 0;
}

static int run(const struct dc_posix_env *env, struct dc_error *err, struct dc_application_settings *settings) {
    struct application_settings *app_settings;
    const char *parity;
    const char *prefix;
    int parity_int;
    char chars[BUF_SIZE];
    ssize_t nread;
    uint8_t byte;
    uint8_t array_of_bits[BITS_PER_BYTE];
    uint8_t array_hamming[4];
    mode_t modes = S_IRUSR | S_IWUSR;
    int fd;

    DC_TRACE(env);

    app_settings = (struct application_settings *) settings;
    parity = dc_setting_string_get(env, app_settings->parity);
    prefix = dc_setting_string_get(env, app_settings->prefix);

    if (!dc_strcmp(env, parity, "odd")) parity_int = 1;
    else if (!dc_strcmp(env, parity, "even")) parity_int = 0;
    else {
        printf("Incorrect parity entered! Either 'even' or 'odd', default is 'even' (case sensitive)\n");
        exit(EXIT_FAILURE);
    }

    const size_t len = dc_strlen(env, prefix) + 11;

    char path[len];

    nread = dc_read(env, err, STDIN_FILENO, chars, BUF_SIZE);
    size_t size = (size_t) nread - 1;

    uint8_t array_bit_0[size];
    uint8_t array_bit_1[size];
    uint8_t array_bit_2[size];
    uint8_t array_bit_3[size];
    uint8_t array_bit_4[size];
    uint8_t array_bit_5[size];
    uint8_t array_bit_6[size];
    uint8_t array_bit_7[size];

    uint8_t array_bit_8[size];
    uint8_t array_bit_9[size];
    uint8_t array_bit_10[size];
    uint8_t array_bit_11[size];

    for (size_t i = 0; i < size; i++) {
        byte = (uint8_t) chars[i];
        generate_array_of_bits_from_character(array_of_bits, BITS_PER_BYTE, byte);
        generate_array_of_hamming_bits(array_hamming, array_of_bits, parity_int);

        array_bit_0[i] = array_of_bits[0];
        array_bit_1[i] = array_of_bits[1];
        array_bit_2[i] = array_of_bits[2];
        array_bit_3[i] = array_of_bits[3];
        array_bit_4[i] = array_of_bits[4];
        array_bit_5[i] = array_of_bits[5];
        array_bit_6[i] = array_of_bits[6];
        array_bit_7[i] = array_of_bits[7];

        array_bit_8[i] = array_hamming[0];
        array_bit_9[i] = array_hamming[1];
        array_bit_10[i] = array_hamming[2];
        array_bit_11[i] = array_hamming[3];
    }

    size_t nbyte = (size_t)(((size - 1) / BITS_PER_BYTE) + 1);
// length of write_file_value depends on number of letters if < mod 8
    char write_file_value[MAX_FILE_LENGTH] = {0};

    for (size_t index = 0; index < BITS_INC_HAMMING; index++) {
        uint8_t writeValue = 0;
        // positions of byte.
        for (size_t pos = 0; pos < nbyte; pos++) {
            writeValue = 0;
            size_t upper_limit;
            if (size < (BITS_PER_BYTE * (pos + 1))) {
                upper_limit = size;
            } else upper_limit = BITS_PER_BYTE * (pos + 1);
            if (index == 0) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_0[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 1) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_1[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 2) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_2[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 3) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {

                    if (array_bit_3[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 4) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_4[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 5) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {

                    if (array_bit_5[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 6) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {

                    if (array_bit_6[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 7) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {

                    if (array_bit_7[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 8) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_8[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 9) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_9[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 10) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_10[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            } else if (index == 11) {
                for (size_t j = BITS_PER_BYTE * pos; j < upper_limit; j++) {
                    if (array_bit_11[j] == 1) {
                        writeValue |= B00000001;
                    }
                    if (j != upper_limit - 1) { writeValue <<= 1; }
                }
            }
            write_file_value[pos] = (char) writeValue;
        }
        snprintf(path, len + 1, "%s_%zu.hamming", prefix, index); // puts string into buffer
        fd = dc_open(env, err, path, DC_O_CREAT | DC_O_TRUNC | DC_O_WRONLY, modes);
        if (dc_error_has_no_error(err)) {
            dc_write(env, err, fd, write_file_value, nbyte);
            dc_dc_close(env, err, fd);
        }
    }

    return EXIT_SUCCESS;
}


static void error_reporter(const struct dc_error *err) {
    fprintf(stderr, "ERROR: %s : %s : @ %zu : %d\n", err->file_name, err->function_name, err->line_number, 0);
    fprintf(stderr, "ERROR: %s\n", err->message);
}

static void trace_reporter(__attribute__((unused)) const struct dc_posix_env *env,
                           const char *file_name,
                           const char *function_name,
                           size_t line_number) {
    fprintf(stdout, "TRACE: %s : %s : @ %zu\n", file_name, function_name, line_number);
}


static void
generate_array_of_hamming_bits(uint8_t array_of_hamming_bits[4], uint8_t array_of_bits[BITS_PER_BYTE], int parity) {
    int sum_of_8s_mod2 = (array_of_bits[4] + array_of_bits[5] + array_of_bits[6] + array_of_bits[7]) % 2;
    int sum_of_4s_mod2 = (array_of_bits[1] + array_of_bits[2] + array_of_bits[3] + array_of_bits[7]) % 2;
    int sum_of_2s_mod2 =
            (array_of_bits[0] + array_of_bits[2] + array_of_bits[3] + array_of_bits[5] + array_of_bits[6]) % 2;
    int sum_of_1s_mod2 =
            (array_of_bits[0] + array_of_bits[1] + array_of_bits[3] + array_of_bits[4] + array_of_bits[6]) % 2;
    // even
    if (parity == 0) {
        if (sum_of_1s_mod2 == 1) array_of_hamming_bits[0] = 1;
        else array_of_hamming_bits[0] = 0;
        if (sum_of_2s_mod2 == 1) array_of_hamming_bits[1] = 1;
        else array_of_hamming_bits[1] = 0;
        if (sum_of_4s_mod2 == 1) array_of_hamming_bits[2] = 1;
        else array_of_hamming_bits[2] = 0;
        if (sum_of_8s_mod2 == 1) array_of_hamming_bits[3] = 1;
        else array_of_hamming_bits[3] = 0;
    } else { // odd
        if (sum_of_1s_mod2 == 1) array_of_hamming_bits[0] = 0;
        else array_of_hamming_bits[0] = 1;
        if (sum_of_2s_mod2 == 1) array_of_hamming_bits[1] = 0;
        else array_of_hamming_bits[1] = 1;
        if (sum_of_4s_mod2 == 1) array_of_hamming_bits[2] = 0;
        else array_of_hamming_bits[2] = 1;
        if (sum_of_8s_mod2 == 1) array_of_hamming_bits[3] = 0;
        else array_of_hamming_bits[3] = 1;
    }
}

static void
generate_array_of_bits_from_character(uint8_t array_of_bits[BITS_PER_BYTE], size_t length, const uint8_t byte) {
    unsigned char masked;
    uint8_t mask;
    for (size_t i = 0; i < length; i++) {
        if (i == 7) mask = B00000001;
        if (i == 6) mask = B00000010;
        if (i == 5) mask = B00000100;
        if (i == 4) mask = B00001000;
        if (i == 3) mask = B00010000;
        if (i == 2) mask = B00100000;
        if (i == 1) mask = B01000000;
        if (i == 0) mask = B10000000;
        masked = byte & mask;
        if (mask == masked) {
            array_of_bits[i] = 1;
        } else {
            array_of_bits[i] = 0;
        }
    }
}
