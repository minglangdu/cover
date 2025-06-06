#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <fftw3.h>

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

vector<double> fft(vector<short> v) {
    int n = v.size();

    fftw_complex *in, *out;
    fftw_plan p;

    // allocate complexes
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    for (int i = 0; i < n; i ++) {
        in[i][1] = 0; // no imaginary part
        in[i][0] = (double) v[i];
    }
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    // create plan and execute
    p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    // clean
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
    // get output
    vector<double> ans (n);
    for (int i = 0; i < n; i ++) {
        ans[i] = out[i][0];
        if (i < 1000) cout << ans[i] << " ";
    }
    cout << "\n";
    return ans;
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
    return 0;
}