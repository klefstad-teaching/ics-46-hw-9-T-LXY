#include "ladder.h"
#include <cmath>

void error(string word1, string word2, string msg) {
    cout << word1 << " and " << word2 << " " << msg << "." << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d){
    int len1 = str1.size();
    int len2 = str2.size();
    int difference = 0;

    if (abs(len1 - len2) > d) return false;

    for (int i = 0, j = 0; difference <= d and i < len1 and j < len2; ++i, ++j){
        if (str1[i] != str2[j]){
            if (len1 < len2) --i;
            else if (len1 > len2) --j;
            ++difference;
        }

        if (i == len1) difference += len2 - j;
        else if (j == len2) difference += len1 - i;
    }

    return difference <= d;
}

bool is_adjacent(const string& word1, const string& word2){ 
    return edit_distance_within(word1, word2, 1);
}

string lower(const string& str){
    string lower(str);
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list){
    string begin = lower(begin_word);
    string end = lower(end_word);

    if (begin_word == end_word or !word_list.contains(end_word)) return {};

    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    set<string> visited;
    visited.insert(begin_word);

    while (!ladder_queue.empty()){
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = ladder.back();

        for (const string& word : word_list){
            string lword = lower(word);
            if (is_adjacent(last_word, lword) && !visited.contains(lword)){
                visited.insert(lword);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(lword);

                if (lword == end_word) return new_ladder;
                ladder_queue.push(new_ladder);
            }
        }
    }

    return {};
}

void load_words(set<string> & word_list, const string& file_name){
    ifstream file(file_name);
    string word;

    while (file >> word)
        word_list.insert(word);

    file.close();
}

void print_word_ladder(const vector<string>& ladder){
    if (ladder.empty()){
        cout << "No word ladder found." << endl;
        return;
    }  

    cout << "Word ladder found: ";

    for (const string& s : ladder)
        cout << s << " ";

    cout << endl;
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
void verify_word_ladder(){
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}