/**
 * @file typedef.cpp
 * @Synopsis  util func
 * @author Saki Shum, sakishum1118@gmail.com
 * @version 0.0.1
 * @date 2013-02-26
 */
#include <iostream>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <fstream>
#include "Tp_typedef.h"
using namespace std;

const string debugDir = "debug_log.txt";

/// write debug log
void Tp_write_to_debug_log(const char *format, ...) {
    char buffer[1024] = {0};

    /// get the va_args
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    /// get the current time
    time_t _t;
    _t = time(NULL);
    char *date = ctime(&_t);

    ofstream debug_log;
    debug_log.open(debugDir.c_str(),ios::app);
    if(debug_log.is_open()) {
		/// strlen need #include <string.h>
        debug_log.write(date, strlen(date)-1);
        debug_log << '\t';
        debug_log << buffer << endl;
        debug_log.close();
    }
}
