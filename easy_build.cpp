/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Bryn Murrell $
   ======================================================================== */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "blib_utils.h"
#include "DebugFileIO.cpp"
#include "len_string.h"
#include "stdlib_files.h"
#include "Windows.h"
#include <time.h>
#include <ctype.h>
#include <unordered_map>

std::unordered_map<len_string, int> already_checked_map;

struct file_build_info
{
    time_t write_time;
    char cpp_name[MAX_PATH];
};

flocal inline u64 file_time_to_unix(FILETIME f)
{
    ULARGE_INTEGER l_int;
    l_int.LowPart = f.dwLowDateTime;
    l_int.HighPart = f.dwHighDateTime;
    u64 time_windows = (u64)l_int.QuadPart;
    u64 c = 11644473600LL;
    u64 m = 10000000;
    u64 time_unix = (time_windows / m - c);
    return time_unix;
}

flocal inline void write_file_build_info(file_build_info info, char* path)
{
    FILE* f_ptr = fopen(path, "wb");
    if (f_ptr)
    {
        fwrite(&info, sizeof(file_build_info), 1, f_ptr);
        fclose(f_ptr);
    }
}

flocal inline file_build_info* read_file_build_info(char* path)
{
    FILE* f_ptr = fopen(path, "rb");
    file_build_info* info = nullptr;
    if (f_ptr)
    {
        info = (file_build_info*)read_entire_file_binary(f_ptr);
        fclose(f_ptr);
    }
    return info;
}

flocal inline file_build_info get_file_build_info(u64 file_time, char* cpp_name)
{
    file_build_info info = {};
    u32 len = strlen(cpp_name);
    memcpy(info.cpp_name, cpp_name, len * sizeof(char));
    info.write_time = file_time;
    return info;
}

flocal inline u32 find_last_occurence_of(char* str, char c)
{
    u32 last_occ = -1;
    u32 ctr = 0;
    while(*str)
    {
        if (*str == c)
        {
            last_occ = ctr;
        }
        ctr++;
        str++;
    }
    return last_occ;
}

b32 is_bat_file(char* path)
{
    while(*path)
    {
        if (*path == '.')
        {
            if (path[1] == 'b' && (path[2] == 0 || (path[2] == 'a' && path[3] == 't')))
            {
                return true;
            }
        }
        path++;
        
    }
    return false;
}

b32 is_cpp_file(char* path)
{
    while(*path)
    {
        if (*path == '.')
        {
            if (path[1] == 'c' && (path[2] == 0 || (path[2] == 'p' && path[3] == 'p')))
            {
                return true;
            }
        }
        path++;
        
    }
    return false;
}

b32 is_h_file(char* path)
{
    while(*path)
    {
        if (*path == '.')
        {
            if (path[1] == 'h' && path[2] == 0)
            {
                return true;
            }
        }
        path++;
        
    }
    return false;
}

b32 is_inl_file(char* path)
{
    while(*path)
    {
        if (*path == '.')
        {
            if (path[1] == 'i' && path[2] == 'n' && path[3] == 'l' && path[4] == 0)
            {
                return true;
            }
        }
        path++;
        
    }
    return false;
}

flocal void to_lower(char* c)
{
    while(*c)
    {
        *c = tolower(*c++);
    }
    
}

b32 should_compile_file(char* file_name, const len_string& parent_file_str, b32* found_in_map)
{
    //  asdasd //TODO info file name should be something like toplevelfile_nestedfile_file.ebi
        //so that we will compile correctly if we have two cpps that include the same file 
        //but are compiled separately
    u32 path_len = find_last_occurence_of(file_name, '\\');
    len_string file_name_only = l_string(&file_name[path_len+1]);
    u32 period_idx = find_first_occurence_of_char(file_name_only, '.');
    file_name_only.str[period_idx] = 0;
    file_name_only.string_len = period_idx;
    if (is_cpp_file(file_name))
    {
        append_to_len_string(&file_name_only, "_cpp");
    }
    else if (is_h_file(file_name))
    {
        append_to_len_string(&file_name_only, "_h");
    }
    else if (is_inl_file(file_name))
    {
        append_to_len_string(&file_name_only, "_inl");
        return false;
    }
    else if (is_bat_file(file_name))
    {
        append_to_len_string(&file_name_only, "_bat");
    }
    if (already_checked_map.find(file_name_only) != already_checked_map.end())
    {
        *found_in_map = true;
        return already_checked_map[file_name_only];
    }
    *found_in_map = false;
    len_string cpp_path = l_string(file_name);
    cpp_path.str[path_len+1] = 0;
    cpp_path.string_len = path_len+1;
    
    len_string data_path = l_string("W:\\easy_build\\Data\\");
    append_to_len_string(&data_path, parent_file_str.str);
    append_to_len_string(&data_path, file_name_only.str);
    append_to_len_string(&data_path, ".ebi");
    file_build_info* info = read_file_build_info(data_path.str);
    HANDLE cpp_file = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL,
                                 OPEN_EXISTING, 0, NULL);
    ASSERT(cpp_file != INVALID_HANDLE_VALUE, "CreateFile failed");
    FILETIME create, access, write;
    GetFileTime(cpp_file, &create, &access, &write);
    u64 time_unix = file_time_to_unix(write);
    if (!info)
    {
        write_file_build_info(get_file_build_info(time_unix, file_name), data_path.str);
        free(info);
        //printf("%s says we should compile!\n", file_name);
        already_checked_map.insert({file_name_only, 1});
        return true;
    }
    else
    {
        if (time_unix > info->write_time)
        {
            write_file_build_info(get_file_build_info(time_unix, file_name), data_path.str);
            free(info);
            
            //printf("%s says we should compile!\n", file_name);
            already_checked_map.insert({file_name_only, 1});
            return true;
        }
        else
        {
            free(info);
            
        already_checked_map.insert({file_name_only, 0});
            return false;
        }
    }
}

flocal inline b32 check_directory_for_file(char* dir, len_string to_find, len_string* out)
{
    len_string s = l_string(dir);
    append_to_len_string(&s, "\\*");
    WIN32_FIND_DATA ffd;    
    HANDLE hFind = FindFirstFile(s.str, &ffd);
    len_string file_name_stripped = to_find;
    if (file_name_stripped.str[0] == '\"')
    {
        file_name_stripped.str = &file_name_stripped.str[1];
        file_name_stripped.string_len -= 2;
    }
    char* split = (char*) malloc(file_name_stripped.string_len+1);
    memset(split, 0, file_name_stripped.string_len+1); 
    memcpy(split, file_name_stripped.str, file_name_stripped.string_len);
    std::vector<len_string> dir_chain;
    std::vector<len_string> sub_directories;
    char* tok = strtok(split, "\\");
    while(tok)
    {
        dir_chain.push_back(l_string(tok));
        tok = strtok(NULL, "\\");
    }
    do
    {
        to_lower(ffd.cFileName);
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && ffd.cFileName[0] != '.')
        {
            LOOP(i, dir_chain.size())
            {
                if (streq(ffd.cFileName,
                          dir_chain[i].str,
                          dir_chain[i].string_len))
                {
                    sub_directories.push_back(dir_chain[i]);
                }
            }
        }
        if(ffd.cFileName[0] != '.' && streq(file_name_stripped.str,
                                                 ffd.cFileName,
                                                 file_name_stripped.string_len))
        {
            *out = l_string(dir);
            append_to_len_string(out, "\\");
            append_to_len_string(out, ffd.cFileName);
#if 0 
            printf("%s\n", ffd.cFileName);
#endif
            return true;
        }
    } while(FindNextFile(hFind, &ffd) != 0);
    LOOP(i, sub_directories.size())
    {
        len_string full_path = l_string(dir);
        append_to_len_string(&full_path, "\\");
        append_to_len_string(&full_path, sub_directories[i].str);
        b32 res = check_directory_for_file(full_path.str, l_string(dir_chain[dir_chain.size()-1]), out);
        if (res)
        {
            return true;
        }
    }
    return false;
    FindClose(hFind);

}

flocal inline b32 check_include_dirs_for_file(const std::vector<char*>& include_dirs, len_string l, len_string* ret)
{
    len_string out;
    LOOP(i, include_dirs.size())
    {
        b32 result = check_directory_for_file(include_dirs[i], l, &out);
        if (result)
        {
            *ret = out;
            return true;
        }
    }
    return false;
}

flocal inline b32 should_compile_recursive(char* file_name, const std::vector<char*>& include_dirs, const len_string& parent_str)
{
    b32 found_in_map;
    b32 should_compile = should_compile_file(file_name, parent_str, &found_in_map);
    if (found_in_map)
    {
        return should_compile;
    }
    u64 cpp_file_size;
    char* cpp_file = read_entire_file_text(file_name, &cpp_file_size);
    while(*cpp_file)
    {
        if (*cpp_file == '#')
        {
            if(streq(&(cpp_file[1]), "include", 7))
            {
                char* cp = &(cpp_file[9]);
                if (cp[0] == '\"')
                {
                    char* cp_copy = cp;
                    while(cp_copy[0] != '\n' && cp_copy[0] != '\r')
                    {
                        cp_copy++;
                    }
                    u32 len = cp_copy - cp;
                    len_string l = l_string(cp, len);
                    to_lower(l.str);
                    for (int i = 0; i < l.string_len; i++)
                    {
                        if (l.str[i] == '/')
                        {
                            l.str[i] = '\\';
                        }
                    }
                    len_string full_dir;
#if 0
                    if (streq(l.str, "\"jump", 5))
                    {
                        printf("ack\n");
                    }
#endif
                    if(check_include_dirs_for_file(include_dirs, l, &full_dir))
                    {
                        b32 compile_temp = should_compile_recursive(full_dir.str, include_dirs, parent_str);
                        if (!should_compile)
                        {
                            should_compile = compile_temp;
                        }
                    }
                }
            }
        }
        cpp_file++;
    }
    return should_compile;
}

int main(u32 argc, char** argv) 
{
    std::vector<char*> include_dirs;
    std::vector<char*> extra_files;
    ASSERT(is_cpp_file(argv[1]), "No CPP file passed.\nUsage is as follows.\neasy_build.exe [cpp_file] -I [include directory] [...] -F [extra file to check] [...]\nyou may list any number of include directories ")
    char* file_name = argv[1];
    for (int i = 2; i < argc; i++)
    {
        if (strlen(argv[i]) == 2 && argv[i][0] == '-' && argv[i][1] == 'I')
        {
            //printf("%s\n",argv[i+1]);
            include_dirs.push_back(argv[i+1]);
        }
        if (strlen(argv[i]) == 2 && argv[i][0] == '-' && argv[i][1] == 'F')
        {
            extra_files.push_back(argv[i+1]);
        }
    }
        
    u32 path_len = find_last_occurence_of(file_name, '\\');
    len_string file_name_only = l_string(&file_name[path_len+1]);
    u32 period_idx = find_first_occurence_of_char(file_name_only, '.');
    file_name_only.str[period_idx] = 0;
    file_name_only.string_len = period_idx;
    
    len_string parent_str = l_string(file_name_only);
    append_to_len_string(&parent_str, "_");
    
    b32 should_compile = false;
    LOOP(i, extra_files.size())
    {
        b32 found_in_map;
        if (should_compile_file(extra_files[i], parent_str, &found_in_map))
        {
            should_compile = true;
        }
    }
    if (should_compile_recursive(file_name, include_dirs, file_name_only))
    {
        should_compile = true;
    }
    
   
    if (should_compile)
    {
        //printf("Compile!\n");
        return 0;
    }
    else
    {
        //printf("Don't compile!\n");
        return 1;
    }

}


