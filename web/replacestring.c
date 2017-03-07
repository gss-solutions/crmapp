/**
 * How to use:
 * ./replacestring "/some/hardcoded/thing/to/change/ "/shorter/thing" < original_file > output_file
 *
 */
#include <stdio.h>
#include <string.h>

typedef struct string_t {
    const char * value;
    size_t length;
} string;

enum { STATE_IN, STATE_OUT, STATE_FILL_SPACE };
struct parser_t {
    string target_text, replace_text;

    char copy_buffer[1024];
    int  copy_buffer_index;

    int remaining_fill_space_length;
    int state;
};

int process_state_fill_space(struct parser_t * parser)
{
    if (parser->remaining_fill_space_length-- > 0) {
        fwrite(" ", sizeof(char), 1, stdout);
        return STATE_FILL_SPACE;
    }
    return STATE_OUT;
}

int process_state_in(struct parser_t * parser, char current_char)
{
    if (parser->copy_buffer_index == parser->replace_text.length) {
        fwrite(parser->replace_text.value, sizeof(char), parser->replace_text.length, stdout);
        parser->copy_buffer_index = 0;

        int remaining_char_size = parser->target_text.length - parser->replace_text.length;

        if (remaining_char_size > 0) {
            parser->remaining_fill_space_length = remaining_char_size;
            return STATE_FILL_SPACE;
        }
        return STATE_OUT;
    }

    if (parser->target_text.value[parser->copy_buffer_index] != current_char) {
        parser->copy_buffer_index = 0;

        return STATE_OUT;
    }

    parser->copy_buffer[parser->copy_buffer_index++] = current_char;
    return STATE_IN;
}

int process_state_out(struct parser_t * parser, char current_char)
{
    if (parser->target_text.value[parser->copy_buffer_index] == current_char) {
        parser->copy_buffer[parser->copy_buffer_index++] = current_char;

        return STATE_IN;
    }

    if (parser->copy_buffer_index > 0) {
        fwrite(parser->copy_buffer, sizeof(char), parser->copy_buffer_index, stdout);
    }
    fwrite(&current_char, sizeof(char), 1, stdout);

    return STATE_OUT;
}

int main(int argc, char *argv[])
{
    char current_char;
    struct parser_t parser;

    memset(&parser, 0, sizeof(struct parser_t));

    if (argc != 3) {
        fprintf(stdout, "%s target_word replace_word < in_file > out_file\t# note in_file and out_file should be different.\n", argv[0]);
        return 0;
    }

    parser.target_text.value = argv[1];
    parser.target_text.length = strlen(argv[1]);
    parser.replace_text.value = argv[2];
    parser.replace_text.length = strlen(argv[2]);
    parser.state = STATE_OUT;

    if (parser.replace_text.length > parser.target_text.length) {
        fprintf(stdout, "replace_text_len cannot be longer than target_len\n");
        return 0;
    }

    if (parser.target_text.length == 0 || parser.replace_text.length == 0) {
        fprintf(stdout, "Neither replace_text nor target_text cannot be zero length string.");
        return 0;
    }

    while (fread(&current_char, sizeof(char), 1, stdin) != 0) {
        switch (parser.state) {
            case STATE_IN:
            {
                parser.state = process_state_in(&parser, current_char);
            }
            break;
            case STATE_OUT:
            {
                parser.state = process_state_out(&parser, current_char);
            }
            break;
            case STATE_FILL_SPACE:
            {
                parser.state = process_state_fill_space(&parser);
            }
            break;
        }
    }
    return 0;
}

