/**
 * The argument checker can be used to check if all mandatory arguments were given when calling the application.
 *
 * @author Pieter Verschaffelt
 */
#ifndef ARGUMENT_CHECKER_H
#define ARGUMENT_CHECKER_H

/**
 * Print the help information to stdout associated with this application.
 */
void print_help();

/**
 * This function can be called whenever a wrong argument was given to this application.
 * The given message will be printed to stdout, together with this command help information.
 */
void wrong_argument(char* message);

/**
 * This function checks if all mandatory options were given by the user. If a command was not
 * given, or in a wrong format, an error message will be shown and the application exits.
 */
void check_arguments(int argc, char** argv);
#endif
