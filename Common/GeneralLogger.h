/*
 *  General configurable logging class
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <cstdarg>
#include <ostream>
#include <iostream>

using namespace std;


class GeneralLogger {
public:
    GeneralLogger() : _debug(&cout), _info(&cout), _warning(&cerr), _error(&cerr), _initiator(NULL), _separator(NULL) {};
    ~GeneralLogger() {};

    // Set loggin streams
    void set_debug_stream(ostream* stream) { if(stream) _debug = stream; }
    void set_info_stream(ostream* stream) { if(stream) _info = stream; }
    void set_warning_stream(ostream* stream) { if(stream) _warning = stream; }
    void set_error_stream(ostream* stream) { if(stream) _error = stream; }

    // Set initiator
    void set_initiator(void (*func)(ostream&)) { if(func) _initiator = func; }
    // Set separator
    void set_separator(void (*func)(ostream&)) { if(func) _separator = func; }

    // Get logging streams
    ostream& get_debug() { return *_debug; }
    ostream& get_info() { return *_info; }
    ostream& get_warning() { return *_warning; }
    ostream& get_error() { return *_error; }

    // Logging functions
    void log_debug(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        if(_initiator) _initiator(*_debug);
        (*_debug) << format_message(fmt, ap);
        if(_separator) _separator(*_debug);
        va_end(ap);
    }
    void log_info(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        if(_initiator) _initiator(*_info);
        (*_info) << format_message(fmt, ap);
        if(_separator) _separator(*_info);
        va_end(ap);
    }
    void log_warning(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        if(_initiator) _initiator(*_warning);
        (*_warning) << format_message(fmt, ap);
        if(_separator) _separator(*_warning);
        va_end(ap);
    }
    void log_error(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        if(_initiator) _initiator(*_error);
        (*_error) << format_message(fmt, ap);
        if(_separator) _separator(*_error);
        va_end(ap);
    }

private:
    // Format log string
    string format_message(const char* fmt, va_list args) {

        size_t size = 1024;
        char* buffer = new char[size];

        while (1) {
            int n = vsnprintf(buffer, size, fmt, args);

            // If that worked, return a string.
            if ((n > -1) && (static_cast<size_t>(n) < size)) {
                string s(buffer);
                delete [] buffer;
                return s;
            }

            // Else try again with more space.
            size = (n > -1) ?
                n + 1 :   // ISO/IEC 9899:1999
                size * 2; // twice the old size

            delete [] buffer;
            buffer = new char[size];
        }
    }

    // Initiator and separator functions
    void (*_initiator)(ostream&);
    void (*_separator)(ostream&);

    // Output streams
    ostream* _debug;
    ostream* _info;
    ostream* _warning;
    ostream* _error;
};
