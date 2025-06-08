#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>

#include "sdl.h"

using namespace std;

vector<short> ppcm(string s) {
    cout << "Opening file " << s << "\n";
    fstream file; file.open(s, ios::in | ios::binary);
    vector<short> v; short cur;
    if (!file.is_open()) {
        cout << "File did not open.";
        throw exception();
    }
    while (true) {
        // PCM is 16-bit little-endian, meaning that there are *2* 8-bit segments
        // reason for use of short, since that is a 2-byte storage format
        file.read((char *)&cur, 2);
        if (file.eof()) break;
        v.push_back(cur);
    }
    return v;
}

const int MIN_SIZE = 50; // minimum size of a sample
const int RUNS = 5; // runs of the function

int main(int argc, char* argv[]) {
    string covered = argv[1];
    string used = argv[2];
    system(("ffmpeg -y  -i " + covered +  " -hide_banner -loglevel error -acodec pcm_s16le -f s16le -ac 1 -ar 16000 input/covered.pcm").c_str());
    system(("ffmpeg -y  -i " + used +  " -hide_banner -loglevel error -acodec pcm_s16le -f s16le -ac 1 -ar 16000 input/used.pcm").c_str());
    cout << "Getting frequencies...\n";
    vector<short> cov = ppcm("input/covered.pcm");
    vector<short> use = ppcm("input/used.pcm");
    cout << "Sizes: " << cov.size() << " " << use.size() << "\n";
    /*---------------*/
    cout << "Preprocessing sums...\n";
    vector<pair<int, int>> sums (use.size() - MIN_SIZE + 1);
    vector<int> psum (cov.size() + 1, 0);
    for (int i = 1; i <= cov.size(); i ++) {
        psum[i] = psum[i - 1] + cov[i - 1];
    }
    int cur = 0;
    for (int i = 0; i < MIN_SIZE - 1; i ++) {
        cur += use[i];
    }
    for (int i = MIN_SIZE - 1; i < sums.size(); i ++) {
        int j = i - MIN_SIZE + 1;
        cur += use[i];
        sums[j].first = cur;
        sums[j].second = j;
        cur -= use[j];
    }
    sort(sums.begin(), sums.end());
    vector<pair<pair<int, int>, pair<int, int>>> mappings;
    cout << "Creating cover...\n";
    int l = 0, r = l;
    vector<short> res (cov.size());
    while (r < cov.size()) {
        r = min((int)cov.size(), l + MIN_SIZE);
        int csum = psum[r + 1] - psum[l];
        
        int section;
        auto higher = lower_bound(sums.begin(), sums.end(), make_pair(csum, 0));
        if (higher == sums.begin()) {
            section = 0;
        } else if (higher == sums.end()) {
            section = sums.size() - 1;
        } else {
            auto lower = prev(higher);
            if (abs((*higher).first - csum) < abs((*lower).first - csum)) {
                section = distance(sums.begin(), higher);
            } else {
                section = distance(sums.begin(), lower);
            }
        }
        for (int i = l; i < r; i ++) {
            res[i] = use[section + i - l];
        }
        mappings.push_back(make_pair(make_pair(l, r), make_pair(section, section + r - l)));
        l = r;
    }
    cout << "Representing as graph...\n";
    SDLH::SoundPlot* s = new SDLH::SoundPlot("Resulting Cover", res);
    bool quit = false;
    while (!quit) {
        quit = s->update();
    }
    delete s;
    s = nullptr;
    return 0;
}