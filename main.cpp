#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include <random>

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

void opcm(string s, vector<short> v) {
    cout << "Opening file " << s << "\n";
    fstream file; file.open(s, ios::out | ios::binary);
    if (!file.is_open()) {
        cout << "File did not open.";
        throw exception();
    }
    for (short cur : v) {
        // PCM is 16-bit little-endian, meaning that there are *2* 8-bit segments
        // reason for use of short, since that is a 2-byte storage format
        file.put(cur & 255); // 255 = ~(1 << 8)
        cur >>= 8;
        file.put(cur & 255);
    }
}

const int MIN_SIZE = 10000; // minimum size of a sample
const int RUNS = 5; // runs of the function
const int SAMPLE_INTERVAL = 1500;

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
    vector<pair<pair<int, int>, pair<int, int>>> mappings;
    cout << "Creating cover...\n";
    int l = 0, r = l;
    vector<short> res (cov.size(), 0);
    while (r < cov.size()) {
        r = min((int)cov.size(), l + MIN_SIZE); // range is [l, r) (r exclusive)
        cout << "Covering range [" << l << ", " << r << ")...\n";
        int section = 0; long long cur = 1e18;
        for (int j = 0; j < use.size() - MIN_SIZE + 1; j += SAMPLE_INTERVAL) {
            pair<int, int> tem = {1e9, -1e9};
            for (int k = 0; k < MIN_SIZE; k ++) {
                int diff = abs((cov[l + k] - use[j + k]));
                tem.first = min(tem.first, diff);
                tem.second = max(tem.second, diff);
            }
            // cout << "try " << j << "\n";
            if ((tem.second - tem.first) < cur) {
                section = j;
                cur = (tem.second - tem.first);
                // cout << "new best: " << section << " (diff " << cur << ")\n";
            }
        }
        cout << "Best: " << section << " (diff " << cur << ")\n";
        for (int i = l; i < r; i ++) {
            res[i] += use[section + i - l];
        }
        mappings.push_back(make_pair(make_pair(l, r), make_pair(section, section + r - l)));
        l = r;
    }
    cout << "Converting to pcm...\n";
    opcm("res.pcm", res);
    system("ffplay  -autoexit -f s16le -ar 16000 res.pcm");
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