#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;
struct Monomial{
    int degree;
    double k;
    string s;
};

struct Polynomial{
    int degree;
    vector <vector <Monomial> > mono;
};

Monomial operator-(const Monomial & m) {
    return {m.degree, -m.k, m.s};
}

Polynomial operator+(const Monomial & m1, const Monomial & m2) {
    int dg = max(m1.degree, m2.degree);
    vector <vector <Monomial> > m(dg + 1);
    if (m1.s == m2.s)
        m[dg].push_back({dg, m1.k + m2.k, m1.s});
    else {
        m[m1.degree].push_back(m1);
        m[m2.degree].push_back(m2);
    }
    return {dg, m};
}

Polynomial operator-(const Monomial & m1, const Monomial & m2) {
    int dg = max(m1.degree, m2.degree);
    vector <vector <Monomial> > m(dg + 1);
    if (m1.s == m2.s)
        m[dg].push_back({dg, m1.k - m2.k, m1.s});
    else {
        m[m1.degree].push_back(m1);
        m[m2.degree].push_back(-m2);
    }
    return {dg, m};
}

Polynomial operator+(const Polynomial & p, const Monomial & m) {
    Polynomial res = p;
    if (m.degree > p.degree) {
        res.degree = m.degree;
        res.mono.resize(m.degree + 1);
        res.mono[m.degree].push_back(m);
        return res;
    }
    else {
        int i = 0;
        while(i < res.mono[m.degree].size() && m.s != res.mono[m.degree][i].s)
            ++i;
        if (i >= res.mono[m.degree].size()) {
            res.mono[m.degree].push_back(m);
        }
        else {
            res.mono[m.degree][i].k += m.k;
        }
        return res;
    }
}

Polynomial operator-(const Polynomial & p, const Monomial & m) {
    Polynomial res = p;
    if (m.degree > p.degree) {
        res.degree = m.degree;
        res.mono.resize(m.degree + 1);
        res.mono[m.degree].push_back(-m);
        return res;
    }
    else {
        int i = 0;
        while(i < res.mono[m.degree].size() && m.s != res.mono[m.degree][i].s)
            ++i;
        if (i >= res.mono[m.degree].size()) {
            res.mono[m.degree].push_back(-m);
        }
        else {
            res.mono[m.degree][i].k -= m.k;
        }
        return res;
    }
}

void operator+=(Polynomial & p1,const Monomial & m2) {
    p1 = p1 + m2;
}

void operator-=(Polynomial & p1,const Monomial & m2) {
    p1 = p1 - m2;
}

Polynomial operator+(const Polynomial & p1, const Polynomial & p2) {
    Polynomial res = p1;
    for (int i = 0; i < p2.mono.size(); ++i) {
        for (int j = 0; j < p2.mono[i].size(); ++j) {
            res += p2.mono[i][j];
        }
    }
    return res;
}

Polynomial operator-(const Polynomial & p1, const Polynomial & p2) {
    Polynomial res = p1;
    for (int i = 0; i < p2.mono.size(); ++i) {
        for (int j = 0; j < p2.mono[i].size(); ++j) {
            res -= p2.mono[i][j];
        }
    }
    return res;
}

Polynomial operator-(const Polynomial & p) {
    Polynomial res = p;
    for (int i = 0; i < res.mono.size(); ++i) {
        for (int j = 0; j < res.mono[i].size(); ++j) {
            res.mono[i][j].k = -res.mono[i][j].k;
        }
    }
    return res;
}

Polynomial operator*(const Polynomial & p1, int k) {
    Polynomial res = p1;
    for (int i = 0; i < p1.mono.size(); ++i) {
        for (int j = 0; j < p1.mono[i].size(); ++j) {
            res.mono[i][j].k *= k;
        }
    }
    return res;
}

string dtos(double d) {
    string res;
    long long t = d * 10000000;
    bool neg = false;
    if (t < 0){
        t = -t;
        neg = true;
    }
    bool fl = true;
    int cnt = 7;
    while (t > 0) {
        if (cnt == 0 && !fl)
            res = '.' + res;
        else if (cnt == 0)
            fl = false;
        if (fl && t % 10 != 0){
            res = (char)(t % 10 + '0') + res;
            fl = false;
        }
        else if(!fl) {
            res = (char)(t % 10 + '0') + res;
        }
        t /= 10;
        --cnt;
    }
    if (abs(d) < 1)
        res = "0." + res;
    if (neg)
        res = '-' + res;
    return res;
}

double stod(string s) {
    int i = 0;
    double res = 0;
    while (i < s.size() && s[i] != '.'){
        res *= 10;
        res += s[i] - '0';
        ++i;
    }
    ++i;
    int ii = i - 1;
    while (i < s.size()) {
        res += (s[i] - '0') / pow(10, i - ii);
        ++i;
    }
    return res;
}

string multiply(const string & s1, const string & s2) {
    string res;
    int i = 0;
    int j = 0;
    while (i < s1.size() && j < s2.size()) {
        if (!isalpha(s1[i])) {
            res += s1[i];
            ++i;
        }
        else if (!isalpha(s2[j])) {
            res += s2[j];
            ++j;
        }
        else if (s1[i] < s2[j]) {
            res += s1[i];
            ++i;
        }
        else if (s2[j] < s1[i]) {
            res += s2[j];
            ++j;
        }
        else {
            res += s1[i];
            int deg = 2;
            res += '^';
            if ((i + 2 < s1.size() && s1[i + 1] == '^') || (j + 2 < s2.size() && s2[j + 1] == '^')) {
                int ii = i + 2;
                int jj = j + 2;
                int deg1 = 0;
                while (ii < s1.size() && (isdigit(s1[ii]) || s1[ii] == '.')) {
                    deg1 *= 10;
                    deg1 += s1[ii] - '0';
                    ++ii;
                }
                deg += deg1 - (deg1 > 1);
                deg1 = 0;
                while (jj < s2.size() && isdigit(s2[jj])) {
                    deg1 *= 10;
                    deg1 += s2[jj] - '0';
                    ++jj;
                }
                deg += deg1 - (deg1 > 1);
            }
            res += dtos(deg);
            ++i;
            ++j;
            while (i < s1.size() && !isalpha(s1[i]))
                ++i;
            while (j < s2.size() && !isalpha(s2[j]))
                ++j;
        }
    }
    while (i < s1.size()){
        res += s1[i];
        ++i;
    }
    while (j < s2.size()){
        res += s2[j];
        ++j;
    }
    return res;
}

Monomial operator*(const Monomial & m1, const Monomial & m2) {
    string s = multiply(m1.s, m2.s);
    return {m1.degree + m2.degree, m1.k * m2.k, s};
}

Polynomial operator*(const Polynomial & p1, const Polynomial & p2) {
    Polynomial res = (Monomial){0, 0, ""} + (Monomial){0, 0, ""};
    for (int i = 0; i < p1.mono.size(); ++i) {
        for (int j = 0; j < p1.mono[i].size(); ++j) {

            for (int ii = 0; ii < p2.mono.size(); ++ii) {
                for (int jj = 0; jj < p2.mono[ii].size(); ++jj) {
                    res += p1.mono[i][j] * p2.mono[ii][jj];
                }
            }
        }
    }
    return res;
}

void operator*=(Polynomial & p1, const Polynomial & p2) {
    p1 = p1 * p2;
}

void operator+=(Polynomial & p1, const Polynomial & p2) {
    p1 = p1 + p2;
}

void operator-=(Polynomial & p1, const Polynomial & p2) {
    p1 = p1 - p2;
}

Polynomial poly(int t) {
    return (Monomial){0, t, ""} + (Monomial){0, 0, ""};
}

string str(const Polynomial & p) {
    string s;
    for (int i = p.degree; i >= 0; --i) {
        for (int j = 0; j < p.mono[i].size(); ++j) {
            if (abs(p.mono[i][j].k) < 0.00000001)
                continue;
            if (p.mono[i][j].k > 0 && s.size() != 0)
                s += '+';
            else if (p.mono[i][j].k < 0)
                s += '-';
            if (abs(p.mono[i][j].k) - 1 > 0.000000001 || p.mono[i][j].s == "")
                s += dtos(abs(p.mono[i][j].k));
            s += p.mono[i][j].s;

        }
    }
    if (s == "")
        return "0";
    return s;
}

void debud_print(Polynomial p){
    for (int i = p.degree; i >= 0; --i) {
        cout << "\n\n" << i << ": ";
        for (int j = 0; j < p.mono[i].size(); ++j) {
            cout << "(" << p.mono[i][j].degree << ", " << p.mono[i][j].k << ", " << p.mono[i][j].s << ");\n";
        }
    }
}
string str(const Monomial & m) {
    return dtos(m.k) + m.s;
}

Monomial read_monom(string s){
    int i = 0;
    double k = 0;
    int deg = 0;
    string bf;
    while (i < s.size() && (isdigit(s[i]) || s[i] == '.')) {
        bf += s[i];
        ++i;
    }
    k = stod(bf);
    int ind = i;
    while (i < s.size()) {
        if (isalpha(s[i])) {
            ++deg;
            ++i;
        }
        else if (isdigit(s[i])){
            int q = 0;
            while (i < s.size() && isdigit(s[i])) {
                q *= 10;
                q += s[i] - '0';
                ++i;
            }
            deg += q - (q > 0);
        }
        else {
            ++i;
        }
    }
    if (k == 0 && s.substr(ind,s.size()).size() > 0)
        k = 1;
    return {deg, k, s.substr(ind,s.size())};
}
bool is_monomch(char c) {
    return (isdigit(c) || isalpha(c) || c == '^');
}
bool is_monom(string s) {
    int i = 0;
    while (i < s.size() && s[i] != '+' && s[i] != '-' && s[i] != '*')
        ++i;
    return i == s.size();
}
bool is_polynom(string s) {
    int i = 0;
    if (s[0] != '~')
        return 0;
    while (i < s.size() && (s[i] == '~' || isdigit(s[i])))
        ++i;
    return i == s.size();
}
bool is_br(string s) {
    if (s[0] != '(' || s.back() != ')')
        return 0;
    int br = 1;
    int i = 1;
    while (i < s.size() && br != 0) {
        if (s[i] == '(')
            ++br;
        else if (s[i] == ')')
            --br;
        ++i;
    }
    return i == s.size();
}

Polynomial solve(string s, vector <Polynomial> po) {
    if (is_br(s)) {
        s.erase(s.begin());
        s.erase(s.end() - 1);
    }
    if (is_polynom(s)) {
        int ind = 0;
        for (int i = 1; i < s.size(); ++i) {
            ind *= 10;
            ind += s[i] - '0';
        }
        if (ind < po.size())
            return po[ind];
        else
            return poly(0);
    }
    if (is_monom(s))
        return (poly(0) + read_monom(s));
    Polynomial res = poly(0);
    int i = 0;
    bool ml = true; // ������ ����� ��������� � ���� c������ �� ����
    char oper = '+';
    while (i < s.size()) {
        string p;
        int br_left = 0;
        while (i < s.size() && (br_left != 0 || (s[i] != '+' && s[i] != '-' && (!ml || s[i] != '*')) ) ) {
            if (s[i] == '(')
                ++br_left;
            else if (s[i] == ')') {
                --br_left;
            }
            p += s[i];
            ++i;
        }
        if (oper == '+') {
            res += solve(p, po);
        }
        else if (oper == '-') {
            res -= solve(p, po);
        }
        else if (oper == '*') {
            res *= solve(p, po);
        }
        if (i == s.size())
            return res;
        else {
            oper = s[i];
            if (oper == '+' || oper == '-')
                ml = false;
            ++i;
        }
    }
    return res;
}
void erase_spaces(string & s) {
    int i = 0;
    while (i < s.size()) {
        if (s[i] <= 32)
            s.erase(i, 1);
        else
            ++i;
    }
}
string add_spaces(const string & s) {
    string res = s;
    for (int i = 0; i < res.size(); ++i) {
        if (res[i] == '+' || res[i] == '-') {
            res.insert(i, " ");
            res.insert(i + 2, " ");
            i += 2;
        }
    }
    return res;
}
string solve_powers(string s) {
    string res = s;
    for (int i = 1; i < res.size(); ++i) {
        if (res[i] == '^' && res[i - 1] == ')') {
            int j = i + 1;
            string exp = "";
            while (j < res.size() && isdigit(res[j])) {
                exp += res[j];
                ++j;
            }
            j = i - 2;
            int br_left = 1;
            string base = ")";
            while (j >= 0 && br_left != 0) {
                if (res[j] == ')')
                    ++br_left;
                else if (res[j] == '(')
                    --br_left;
                base += res[j];
                --j;
            }
            base += '*';
            reverse(base.begin(), base.end());
            //cout << res << "\n";
            res.erase(i, 1);
            for (int k = 0; k < exp.size(); ++k) {
                //cout << res << "\n";
                res.erase(i, 1);
            }
            for (int k = 0; k < stod(exp) - 1; ++k) {
                //cout << res << "\n";
                res.insert(i, base);
            }
        }
    }
    return res;
}

int main()
{
    ofstream fout("res.txt");
    cout << "1)Enter expression with latin variables.\n2)In each Monomial variables must be in lexicographical order.\n  (Otherwise you may get some bullshit\1)\n";
    //cout << "3)You can also enter an expression in a file named \"file.txt\".\n  Located in the same folder as executable file.\n";
    cout << "3)You may use * + - ( ) ^ .\n  Example: (y + (3.2x^2 + y)*2 - 1)^2 + z\n\n            3.2*(x + y)  NOT 3.2(x + y) \n";
    cout << "4)You can copy results from \"res.txt\"\n5)/help for help\n\n";
    string f, command;
    vector <Polynomial> p;
    getline(cin, command);
    while (command.substr(0, 5) != "/exit") {
        if (command.substr(0, 5) == "/add " || command.substr(0, 3) == "/a ") {
            while (0 < command.size() && command[0] != ' ')
                command.erase(0, 1);
            f = command;
            command = "";
            erase_spaces(f);
            cout << "~" << p.size() << " added\n";
            p.push_back(solve(solve_powers(f), p));
        }
        else if (command.substr(0, 7) == "/print " || command.substr(0, 3) == "/p ") {
            while (0 < command.size() && command[0] != ' ')
                command.erase(0, 1);
            f = command;
            erase_spaces(f);
            if (f.size()) {
                command = "";
                erase_spaces(f);
                cout << add_spaces(str(solve(solve_powers(f), p))) << "\n";
                fout << add_spaces(str(solve(solve_powers(f), p))) << "\n";
            }
            else if (p.size()){
                cout << add_spaces(str(p.back())) << "\n";
                fout << add_spaces(str(p.back())) << "\n";
            }
        }

        else if (command.substr(0, 4) == "/all") {
            for (int i = 0; i < p.size(); ++i) {
                cout << "~" << i << " = " << add_spaces(str(p[i])) << "\n";
                fout << "~" << i << " = " << add_spaces(str(p[i])) << "\n";
            }
        }
        else if (command.substr(0, 7) == "/addsum") {
            Polynomial res = poly(0);
            for (int i = 0; i < p.size(); ++i) {
                res += p[i];
            }
            cout << "~" << p.size() << " added\n";
            p.push_back(res);
        }
        else if (command.substr(0, 9) == "/printsum") {
            Polynomial res = poly(0);
            for (int i = 0; i < p.size(); ++i) {
                res += p[i];
            }
            cout << "Sum = " << add_spaces(str(res)) << "\n";
            fout << "Sum = " << add_spaces(str(res)) << "\n";
        }

        else if (command.substr(0, 8) == "/addprod") {
            Polynomial res = poly(1);
            for (int i = 0; i < p.size(); ++i) {
                res *= p[i];
            }
            cout << "~" << p.size() << " added\n";
            p.push_back(res);
        }
        else if (command.substr(0, 10) == "/printprod") {
            Polynomial res = poly(1);
            for (int i = 0; i < p.size(); ++i) {
                res *= p[i];
            }
            cout << "Product = " << add_spaces(str(res)) << "\n";
            fout << "Product = " << add_spaces(str(res)) << "\n";
        }
        else if (command.substr(0, 5) == "/del ") {
            command.erase(0, 5);
            if (!command.size())
                p.erase(p.begin(), p.end());
            int ind = 0;
            for (int i = (command[0] == '~'); i < command.size(); ++i) {
                ind *= 10;
                ind += command[i] - '0';
            }
            if (ind < p.size())
                p.erase(p.begin() + ind);
        }
        else if (command.substr(0, 5) == "/help") {
            cout << "\nCommands:\n/add\n/exit\n/deg [polynomial]\n/print [polynomial]\n/addfromfile [path]\n/all\n/help\n/del [polynomial index]\n/addsum\n/addprod\n/printsum\n/printprod\nTo call your polynomial enter ~[polynomial index]\n";
        }

        else if (command.substr(0, 5) == "/deg ") {
            while (0 < command.size() && command[0] != ' ')
                command.erase(0, 1);
            f = command;
            command = "";
            erase_spaces(f);
            cout << "deg = " << solve(solve_powers(f), p).degree << "\n";
            fout << "deg = " << solve(solve_powers(f), p).degree << "\n";
        }

        else if (command.substr(0, 13) == "/addfromfile " || command.substr(0, 4) == "/ff ") {
            while (0 < command.size() && command[0] != ' ')
                command.erase(0, 1);
            command.erase(0, 1);
            string path = command;
            if (path == "")
                path = "file.txt";
            command = "";
            ifstream fin(path);
            while (!fin.eof()) {
                getline (fin, f);
                erase_spaces(f);
                cout << "~" << p.size() << ", ";
                p.push_back(solve(solve_powers(f), p));
            }
            cout << (char)8 << (char)8 << " added\n";
        }
        else
            cout << "Undefined command\n/help for help\n";
        getline(cin, command);
    }
    fout.close();
}
