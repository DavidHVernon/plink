#include <string>
#include <iostream>
#include <cstdlib>

#include "plink.h"

using namespace std;

// Attempt to parse args into a plink object and return it optionally.
optional<Plink> build_plink(int argc, const char * argv[]) noexcept {
    // Usage: plink [result_code sound ... [ default_sound ] -- ] command ...
    
    // Convert argv to vector<string> and figure out if we have optional
    // prefix.
    auto args = vector<string>();
    bool has_result_list = false;
    for(int i = 0; i < argc; i++) {
        auto arg = string(argv[i]);
        args.push_back(arg);
        if(arg == "--") {
            has_result_list = true;
        }
    }
    
    // Split args into response (optional prefix) and command.
    vector<string> response_args = vector<string>();
    vector<string> command_args = vector<string>();
    
    if (has_result_list) {
        bool found_divider = false;
        for(string arg : args) {
            if (arg == "--") {
                found_divider = true;
                continue;
            }
            if (found_divider) {
                command_args.push_back(arg);
            } else {
                response_args.push_back(arg);
            }
        }
    } else {
        command_args = args;
    }
    
    // Parse response args.
    auto options = vector<Response>();
    for(int i = 0; i < response_args.size(); i++) {
        string sound = response_args[i*2+1];
        optional<int> result = to_int(response_args[i*2]);
        if(!result.has_value()) {
            // Error
            error("Could not convert arg '" + to_string(i*2) + "' to a result code.");
            return optional<Plink>();
        }
        options.push_back(Response(result, sound));
    }
    if(response_args.size() % 2) {
        // Odd number of params means there is a default specified.
        options.push_back(Response(optional<int>(), response_args.back()));
    }

    // Parse command args.
    string command = string("");
    bool is_first = true;
    for(string arg : command_args) {
        if (!is_first) {
            command += " ";
        } else {
            is_first = false;
        }
        command += arg;
    }
        
    return optional<Plink>(Plink(options, command));
}

int Plink::exec(void) {
    // Shell out the command.
    int command_result = system(this->command.c_str());
    
    // Make a sound.
    for(auto response : this->responses) {
        auto result = response.get_result();
        if(result.has_value()) {
            if (result.value() == command_result) {
                play(response.get_sound());
            }
        } else {
            // Default
            
        }
    }
    
    return 0;
}

void error(string error_message) {
    cerr << error_message << "\n";
}

void Plink::play(string sound) {
    cout << "Time to play " + sound;
}

// Return the string converted to int or none.
optional<int> to_int(string s) {
    char* p;
    long int int_value = strtol(s.c_str(), &p, 10);
    if(*p && (int_value < INT_MAX) && (int_value > INT_MIN)) {
        return optional<int>(int_value);
    }
    return optional<int>();
}
