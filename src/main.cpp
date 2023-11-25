#ifdef _WIN32
#define TX_COMPILED
#define TX_USE_SPEAK
#include "TXLib.h"
#endif //< #ifdef _WIN32

#include "config.h"

#include "log/log.h"

LogFileData log_file = {"log"};

#include "utils/statuses.h"
#include "utils/args_parser.h"
#include "akinator.h"

int main(int argc, char* argv[]) {

#ifdef _WIN32
    txSetLocale();
    txDisableAutoPause();
#endif //< #ifdef _WIN32

    ArgsVars args_vars = {};
    args_vars.input_filename = "trees/example.txt"; //< default value

    STATUS_CHECK_RAISE(args_parse(argc, argv, &args_vars));

    STATUS_CHECK_RAISE(akinator_process(args_vars.input_filename));

    return Status::OK_EXIT;
}
