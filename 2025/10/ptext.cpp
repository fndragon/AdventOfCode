#include <iostream>
#include <vector>
#include <cassert>
using namespace std;
vector<string> split (string s, char delim) {
    vector<string> ans;
    string cur;
    for (char c : s) {
        if (c == delim) {
            ans.push_back(cur);
            cur = "";
        } else {
            cur += c;
        }
    }
    ans.push_back(cur);
    return ans;
}
int parse_config (string s) {
    assert (s[0] == '[');
    assert (s.back() == ']');

    int n = (int)s.size() - 2;
    return n;
}
int parse_toggle (string s) {
    assert (s[0] == '(');
    assert (s.back() == ')');
    auto parts = split(s.substr(1, (int)s.length() - 2), ',');
    int answer = 0;
    for (auto p : parts) {
        answer += (1 << stoi(p));
    }
    return answer;
}
vector<int> parse_joltage (string s) {
    assert (s[0] == '{');
    assert (s.back() == '}');
    auto parts = split(s.substr(1, (int)s.size() - 2), ',');
    vector<int> answer;
    for (auto p : parts) {
        answer.push_back(stoi(p));
    }
    return answer;
}
int answer, N;
vector<int> contain, need, t;
void rec (int mask, int cur_taken) {
    // cout << "CURRENT IT: " << id << " " << cur_taken << endl;
    // for (int p : contain) cout << p << " "; cout << endl;
    // for (int p : need) cout << p << " "; cout << endl;
    if (mask == (1 << N) - 1) {
        for (int p : need) {
            assert (!p);
        }
        if (cur_taken < answer) {
            cout << "FOUND " << cur_taken << endl;
            answer = cur_taken;
        }
        return;
    }

    int best_ret = *max_element(need.begin(), need.end());
    if (best_ret + cur_taken > answer) return;
    int best_id, best_cnt = INT_MAX;
    for (int i = 0; i < N; i++) {
        if (mask & (1 << i)) continue;
        int cnt = INT_MAX;
        for (int j = 0; j < (int)need.size(); j++) {
            if (t[i] & (1 << j)) {
                cnt = min(cnt, contain[j]);
            }
        }
        if (cnt < best_cnt) {
            best_cnt = cnt;
            best_id = i;
        }
    }

    vector<int> me;
    for (int i = 0; i < (int)need.size(); i++) {
        if (t[best_id] & (1 << i)) me.push_back(i);
    }

    if (best_cnt == 1) {
        int must_value = -1;
        for (int p : me) {
            if (contain[p] == 1) {
                if (must_value == -1) must_value = need[p];
                else if (must_value != need[p]) {
                    return;
                }
            }
        }

        assert (must_value != -1);
        bool bad = false;
        for (int p : me) {
            if (must_value > need[p]) {
                bad = true;
                break;
            }
        }
        if (bad) return;
        for (int p : me) {
            need[p] -= must_value;
            contain[p] -= 1;
        }
        rec(mask | (1 << best_id), cur_taken + must_value);
        for (int p : me) {
            need[p] += must_value;
            contain[p] += 1;
        }
        return;
    }
    
    int mx = INT_MAX;
    for (int p : me) {
        mx = min(mx, need[p]);
        contain[p] -= 1;
    }
    for (int p : me) need[p] -= mx;
    rec(mask | (1 << best_id), cur_taken + mx);
    for (int take = 1; take <= mx; take++) {
        for (int p : me) {
            need[p] += 1;
        }
        rec(mask | (1 << best_id), cur_taken + mx - take);
    }
    for (int p : me) {
        contain[p] += 1;
    }
}
int solve (int n, vector<int> t, const vector<int> &goal) {
    N = (int)t.size();
    need.resize(goal.size());
    for (int i = 0; i < (int)goal.size(); i++) need[i] = goal[i];
    contain.clear();
    contain.resize(need.size(), 0);
    for (int p : t) {
        for (int i = 0; i < (int)goal.size(); i++) {
            if (p & (1 << i)) {
                contain[i] += 1;
            }
        }
    }
    ::t = t;
    answer = INT_MAX;
    rec(0, 0);
    return answer;
}
int main() {
    int answer = 0;
    string line;
    int line_cnt = 0;
    while (getline(cin, line)) {
        auto spl = split(line, ' ');
        int n = parse_config(spl[0]);
        vector<int> toggles;
        for (int i = 1; i + 1 < (int)spl.size(); i++) {
            toggles.push_back(parse_toggle(spl[i]));
        }
        auto joltage = parse_joltage(spl.back());
        answer += solve(n, toggles, joltage);
        cerr << "done " << line_cnt++ << endl;
        cerr << "answer so far: " << answer << endl;
    }
    cout << answer << endl;
    return 0;
}