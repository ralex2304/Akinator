#include "TreeDebug/TreeDebug.h"
#include "log/log.h"

#include "utils/statuses.h"
#include "utils/args_parser.h"
#include "akinator.h"

LogFileData log_file = {"log"};

int main(int argc, char* argv[]) {
    ArgsVars args_vars = {};
    args_vars.input_filename = "trees/example.txt"; //< default value

    STATUS_CHECK_RAISE(args_parse(argc, argv, &args_vars));

    STATUS_CHECK_RAISE(akinator_process(args_vars.input_filename));

    return Status::OK_EXIT;
}
