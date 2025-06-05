#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<short> ppcm(string s) {
    cout << "Opening file " << s << "\n";
    fstream file; file.open(s, ios::in | ios::binary);
    vector<short> v; short cur;
    int i = 0;
    if (!file.is_open()) {
        cout << "File did not open.";
        throw exception();
    }
    while (true) {
        file.read((char *)&cur, sizeof(cur));
        if (file.eof()) break;
        v.push_back(cur);
        if (i < 500) cout << cur << " ";
        i ++;
    }
    return v;
}

int main(int argc, char* argv[]) {
    string covered = argv[1];
    string used = argv[2];
    system(("ffmpeg -y  -i " + covered +  " -acodec pcm_s16le -f s16le -ac 1 -ar 16000 input/covered.pcm").c_str());
    system(("ffmpeg -y  -i " + used +  " -acodec pcm_s16le -f s16le -ac 1 -ar 16000 input/used.pcm").c_str());
    cout << "Getting frequencies...\n";
    vector<short> cov = ppcm("input/covered.pcm");
    vector<short> use = ppcm("input/used.pcm");
    return 0;
}