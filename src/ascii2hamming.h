//
// Created by toni on 2021-10-14.
//

#pragma once

#include <dc_application/command_line.h>
#include <dc_application/config.h>
#include <dc_application/defaults.h>
#include <dc_application/environment.h>
#include <dc_application/options.h>
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/dc_fcntl.h>
#include <dc_posix/dc_unistd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>


#define NUMBER_HAMMING_BITS 4
#define BITS_PER_BYTE 8
#define BITS_INC_HAMMING 12
#define MAX_FILE_LENGTH 4196
#define BUF_SIZE 1024
const uint8_t B10000000 = UINT8_C(0x80);
const uint8_t B01000000 = UINT8_C(0x40);
const uint8_t B00100000 = UINT8_C(0x20);
const uint8_t B00010000 = UINT8_C(0x10);
const uint8_t B00001000 = UINT8_C(0x08);
const uint8_t B00000100 = UINT8_C(0x04);
const uint8_t B00000010 = UINT8_C(0x02);
const uint8_t B00000001 = UINT8_C(0x01);

struct application_settings {
    struct dc_opt_settings opts;
    struct dc_setting_string *parity;
    struct dc_setting_string *prefix
};

static struct dc_application_settings *create_settings(const struct dc_posix_env *env, struct dc_error *err);

static int destroy_settings(const struct dc_posix_env *env,
                            struct dc_error *err,
                            struct dc_application_settings **psettings);

static int run(const struct dc_posix_env *env, struct dc_error *err, struct dc_application_settings *settings);

static void error_reporter(const struct dc_error *err);

static void trace_reporter(const struct dc_posix_env *env,
                           const char *file_name,
                           const char *function_name,
                           size_t line_number);


/**
 * Creates an array of size 8 from a single character that represents that character's
 * binary value.
 * @param array_of_bits the array that represents binary value of a character
 * @param length length bits per byte
 * @param byte the character to be "binarized"
 */
static void generate_array_of_bits_from_character(uint8_t array_of_bits[BITS_PER_BYTE], size_t length, uint8_t byte);

/**
 * Creates and add values to an array of size 4 that represents 4 parity bits of a character.
 * @param array_of_hamming_bits the array that represents 4 parity bits of a character
 * @param array_of_bits the array that represents binary value of a character
 * @param parity value, 0 for even, 1 for odd
 */
static void
generate_array_of_hamming_bits(uint8_t array_of_hamming_bits[NUMBER_HAMMING_BITS], uint8_t array_of_bits[BITS_PER_BYTE],
                               int parity);

/**
 * Gets the hamming files' size in bytes.
 * @param env Current working environment
 * @param err Error tracking
 * @param prefix the prefix chosen
 * @param len the length of the files' names
 * @return
 */
static size_t getFileSizeInBytes(const struct dc_posix_env *env,
                                 struct dc_error *err,
                                 const char *prefix,
                                 size_t len);

/**
 * Inserts items to arrays. At position messageIndex % 8 per nbytes byte (position 0->7)
 * from index file (0->11 files), extract the bit value of the current position and add it
 * to array_bits array of bits or add to hamming_bits array of parity bits.
 * Example, index file from 0 to 7, the bit will be inserted to an array of size 8 to represent a byte of
 * a letter, index file from 8 to 11, the current bit will be inserted to an array of size 4 to represent
 * 4 parity bits of the current letter.
 * @param messageIndex current message index
 * @param index current file index, from 0 to 11 (inclusive)
 * @param nbytes value of the current byte working on
 * @param numOfCharsInMessage is the value of 8 * nbytes
 * @param word the message taken from the file
 * @param array_bits array of 8 integers that represent a single byte in a file at position nbytes - 1
 * @param hamming_bit array of 4 integers that represent 4 parity bits of a letter
 */
static void insertItemToArray(size_t messageIndex,
                              size_t index,
                              size_t nbytes,
                              size_t numOfCharsInMessage,
                              char word[MAX_FILE_LENGTH],
                              uint8_t array_bits[],
                              uint8_t hamming_bit[]);

/**
 * Checks whether a bit is a 1 or a 0. Returns 1 if the bit is 1, 0 if
 * bit is 0.
 * @param byteToCheck uint8_t checks whether a bit is 1 or 0
 * @return 1 if the bit is 1, 0 if the bit is 0
 */
static uint8_t checkBit1Or0(uint8_t byteToCheck);

static bool handleErrorDetectionAndPrint(const struct dc_posix_env *env, int parity_int, uint8_t array_bits[],
                                         uint8_t hamming_bits[]);

/**
 * Flips the given bit to the opposite. If the given bit is 0, flip it to 1,
 * and vice versa.
 * @param bitToFlip uint8_t bit represented in type uint8_t to be flipped
 * @return uint8_t the bit after flipped
 */
static uint8_t flipBit(uint8_t bitToFlip);