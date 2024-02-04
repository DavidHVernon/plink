#ifndef args_h
#define args_h

#include <stdio.h>
#include <optional>
#include <string>
#include <vector>

using namespace std;

class Plink;

optional<Plink> build_plink(int arc, const char * argv[]) noexcept;

class Response {

public:
    Response(optional<int> result, string sound ) {
        this->result = result;
        this->sound = sound;
    }
    
    optional<int> get_result(void) { return this->result; };
    string get_sound(void) { return this->sound; };
    
protected:
    // A result of type none is a wildcard, matching any result code.
    optional<int> result;
    // The name of the sound, excluding any file extension.
    string sound;

};

class Plink {

public:
    Plink(vector<Response> responses, string command) {
        this->responses = responses;
        this->command = command;
    }
    
    int exec(void);
    
protected:
    vector<Response> responses;
    string command;

    void play(string sound);

};

optional<int> to_int(string s);

void error(string error_message);

#endif

