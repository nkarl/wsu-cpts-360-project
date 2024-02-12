#include "../../hdr-locked/constants.hh"
#include "../../hdr-locked/my-fs.hh"
#include "../../hdr-locked/my-types.hh"

#include "all-tests.hh"

#include <cassert>

using std::string;

/**
 * Integration test. create a new disk and read from it.
 */
bool _test_fs_ext2_utils() {
    /**
     * SET UP
     */
    string         path = "/a/b/c/d";
    vector<string> path_tokens;

    /**
     * TEST BODY
     */
    {
        stringstream ss(path);
        string       token;
        FS::Utils::tokenize_path(path.c_str(), path_tokens);
        for (auto i = 1; getline(ss, token, '/'); ++i) {
            assert(token == path_tokens[i]);
        }
    }

    path        = "a/b/c/d";
    path_tokens = {};
    {
        stringstream ss(path);
        string       token;
        FS::Utils::tokenize_path(path.c_str(), path_tokens);
        for (auto i = 0; getline(ss, token, '/'); ++i) {
            assert(token == path_tokens[i]);
        }
    }
    /**
     * TEAR DOWN
     */
    return true;
}

/**
 * test executor.
 */
void test_fs_ext2_utils() {
    Test::Header("tokenize path /a/b/c/d");
    bool result = Test::Body(*_test_fs_ext2_utils);
    Test::Footer(result);
}
