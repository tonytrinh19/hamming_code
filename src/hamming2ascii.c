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
    info = dc_application_info_create(&env, &err, "Hamming Code to ASCII Application");
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
    bool error_in_message;
    DC_TRACE(env);
    int return_value = EXIT_SUCCESS;
    // parity int 0 for even, 1 for odd.
    int parity_int;
    int fd;

    app_settings = (struct application_settings *) settings;
    parity = dc_setting_string_get(env, app_settings->parity);
    prefix = dc_setting_string_get(env, app_settings->prefix);

    const size_t len = dc_strlen(env, prefix) + 11;
    char path[len];

    if (!dc_strcmp(env, parity, "odd")) parity_int = 1;
    else if (!dc_strcmp(env, parity, "even")) parity_int = 0;
    else {
        printf("Incorrect parity entered! Either 'even' or 'odd', default is 'even' (case sensitive)\n");
        exit(EXIT_FAILURE);
    }
    // get number of bytes in file
    size_t size = getFileSizeInBytes(env, err, prefix, len);

    uint8_t array_bits[BITS_PER_BYTE] = {0};
    uint8_t hamming_bits[NUMBER_HAMMING_BITS] = {0};

    for (size_t i = 0; i < BITS_PER_BYTE * size; i++) {
        // Opens 12 files for reading
        for (size_t index = 0; index < BITS_INC_HAMMING; index++) {
            snprintf(path, len + 1, "%s_%zu.hamming", prefix, index); // puts string into buffer
            fd = dc_open(env, err, path, DC_O_RDONLY, 0);

            if (dc_error_has_no_error(err)) {
                ssize_t nread;
                char word[MAX_FILE_LENGTH];
                // read the opened file
                if ((nread = dc_read(env, err, fd, word, MAX_FILE_LENGTH)) > 0) {
                    if (dc_error_has_error(err)) {
                        return_value = 1;
                        break;
                    }
                    insertItemToArray(i, index,
                                      size,
                                      BITS_PER_BYTE * size,
                                      word,
                                      array_bits,
                                      hamming_bits);
                }
            }
        }

        // Error Detection Code
        error_in_message = handleErrorDetectionAndPrint(env, parity_int, array_bits, hamming_bits);
    }
    if (error_in_message) {
        printf("\nThis message might have been altered due to corrupted files.\n");
    }
    return return_value;
}

static bool handleErrorDetectionAndPrint(const struct dc_posix_env *env, int parity_int, uint8_t array_bits[],
                                         uint8_t hamming_bits[]) {
    bool error_in_message = false;
    int sum_of_8s_mod2 = (array_bits[4] + array_bits[5] + array_bits[6] + array_bits[7]) % 2;
    int sum_of_4s_mod2 = (array_bits[1] + array_bits[2] + array_bits[3] + array_bits[7]) % 2;
    int sum_of_2s_mod2 =
            (array_bits[0] + array_bits[2] + array_bits[3] + array_bits[5] + array_bits[6]) % 2;
    int sum_of_1s_mod2 =
            (array_bits[0] + array_bits[1] + array_bits[3] + array_bits[4] + array_bits[6]) % 2;

    int errors = 0;
    bool error_pos_8 = false;
    bool error_pos_4 = false;
    bool error_pos_2 = false;
    bool error_pos_1 = false;
    uint8_t value = 0;
    uint8_t bit_mask = B00000001;


    if ((sum_of_8s_mod2 + hamming_bits[3]) % 2 == !parity_int) {
        errors++;
        error_pos_8 = true;
    }
    if ((sum_of_4s_mod2 + hamming_bits[2]) % 2 == !parity_int) {
        error_pos_4 = true;
        errors++;
    }
    if ((sum_of_2s_mod2 + hamming_bits[1]) % 2 == !parity_int) {
        error_pos_2 = true;
        errors++;
    }
    if ((sum_of_1s_mod2 + hamming_bits[0]) % 2 == !parity_int) {
        error_pos_1 = true;
        errors++;
    }

    if (errors == 2 || errors == 0) {
        if (error_pos_1 && error_pos_2) {

            array_bits[0] = flipBit(array_bits[0]);

        } else if (error_pos_1 && error_pos_4) {

            array_bits[1] = flipBit(array_bits[1]);

        } else if (error_pos_1 && error_pos_8) {

            array_bits[4] = flipBit(array_bits[4]);

        } else if (error_pos_2 && error_pos_4) {

            array_bits[2] = flipBit(array_bits[2]);

        } else if (error_pos_2 && error_pos_8) {

            array_bits[5] = flipBit(array_bits[5]);

        } else if (error_pos_4 && error_pos_8) {

            array_bits[7] = flipBit(array_bits[7]);

        }
    } else {
        error_in_message = true;
    }
    for (size_t q = 0; q < BITS_PER_BYTE; q++) {
        if (array_bits[q] == 1) {
            value |= bit_mask;
        }
        if (q != 7) {
            value <<= 1;
        }
    }
    if (isprint(value)) {
        printf("%c", (char) value);
    }
    return error_in_message;
}

static uint8_t flipBit(uint8_t bitToFlip) {
    if (bitToFlip == 0) {
        return 1;
    }
    return 0;
}

static void insertItemToArray(size_t messageIndex,
                              size_t index,
                              size_t nbytes,
                              size_t numOfCharsInMessage,
                              char word[MAX_FILE_LENGTH],
                              uint8_t array_bits[],
                              uint8_t hamming_bit[]) {
    uint8_t byte = (uint8_t) word[messageIndex / BITS_PER_BYTE];

    // if last byte of reading file
    if ((messageIndex / BITS_PER_BYTE) + 1 == nbytes) {
        messageIndex += (BITS_PER_BYTE - (numOfCharsInMessage - BITS_PER_BYTE * (nbytes - 1)));
    }
    uint8_t add;
    if (messageIndex % BITS_PER_BYTE == 0) {
        byte &= B10000000;
        byte |= 0;
        add = checkBit1Or0(byte);
    } else if (messageIndex % BITS_PER_BYTE == 1) {
        byte &= B01000000;
        byte |= 0;
        add = checkBit1Or0(byte);
    } else if (messageIndex % BITS_PER_BYTE == 2) {
        byte &= B00100000;
        byte |= 0;
        add = checkBit1Or0(byte);
    } else if (messageIndex % BITS_PER_BYTE == 3) {
        byte &= B00010000;
        byte |= 0;
        add = checkBit1Or0(byte);
    } else if (messageIndex % BITS_PER_BYTE == 4) {
        byte &= B00001000;
        byte |= 0;
        add = checkBit1Or0(byte);
    } else if (messageIndex % BITS_PER_BYTE == 5) {
        byte &= B00000100;
        byte |= 0;
        add = checkBit1Or0(byte);
    } else if (messageIndex % BITS_PER_BYTE == 6) {
        byte &= B00000010;
        byte |= 0;
        add = checkBit1Or0(byte);
    } else if (messageIndex % BITS_PER_BYTE == 7) {
        byte &= B00000001;
        byte |= 0;
        add = checkBit1Or0(byte);
    }

    if (index <= 7) {
        array_bits[index] = add;
    } else {
        hamming_bit[index - BITS_PER_BYTE] = add;
    }
}

static uint8_t checkBit1Or0(uint8_t byteToCheck) {
    if (byteToCheck == 0) {
        return 0;
    } else {
        return 1;
    }
}

static size_t getFileSizeInBytes(const struct dc_posix_env *env,
                                 struct dc_error *err,
                                 const char *prefix,
                                 size_t len) {
    int fd;
    char path[len];
    snprintf(path, len + 1, "%s_0.hamming", prefix); // puts string into buffer
    fd = dc_open(env, err, path, DC_O_RDONLY, 0);
    if (dc_error_has_no_error(err)) {
        ssize_t nread;
        char word[MAX_FILE_LENGTH];
        // read the opened file
        if ((nread = dc_read(env, err, fd, word, MAX_FILE_LENGTH)) > 0) {
            if (dc_error_has_error(err)) {
                exit(1);
            }
            return (size_t) nread;
        }
    }
    exit(EXIT_FAILURE);
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
