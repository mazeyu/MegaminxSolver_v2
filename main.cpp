#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include<sstream>
#include <fstream>
#include <ctime>
using namespace std;
#define rep(i, l, r) for (int i = l; i < r; i++)
#define mp make_pair
#define XX 1108
typedef vector<string> alg;

struct state {
    int a[132];

    state() {
        rep(i, 0, 132) a[i] = 0;
    }

    bool operator<(const state &s) const {
        rep(i, 0, 132) {
            if (a[i] < s.a[i]) return true;
            if (a[i] > s.a[i]) return false;
        }
        return false;
    }

    bool operator==(const state &s) const {
        rep(i, 0, 132) if (a[i] != s.a[i]) return false;
        return true;
    }

};

double vertex[20][3] = {{-1.37638,  0.,        0.262866},
                        {1.37638,
                                    0.,        -0.262866},
                        {-0.425325, -1.30902,  0.262866},
                        {-0.425325,
                                    1.30902,   0.262866},
                        {1.11352,   -0.809017, 0.262866},
                        {1.11352,
                                    0.809017,  0.262866},
                        {-0.262866, -0.809017, 1.11352},
                        {-0.262866,
                                    0.809017,  1.11352},
                        {-0.688191, -0.5,      -1.11352},
                        {-0.688191,
                                    0.5,       -1.11352},
                        {0.688191,  -0.5,      1.11352},
                        {0.688191,  0.5,
                                               1.11352},
                        {0.850651,
                                    0.,        -1.11352},
                        {-1.11352,  -0.809017, -0.262866},
                        {-1.11352,
                                    0.809017,  -0.262866},
                        {-0.850651, 0.,
                                               1.11352},
                        {0.262866,  -0.809017, -1.11352},
                        {0.262866,
                                    0.809017,  -1.11352},
                        {0.425325,  -1.30902,  -0.262866},
                        {0.425325,
                                    1.30902,   -0.262866}};

int faces[12][5] = {{15, 10, 9,  14, 1},
                    {2,  6,  12, 11, 5},
                    {5,  11, 7,  3,  19},
                    {11, 12, 8,
                                 16, 7},
                    {12, 6,  20, 4,  8},
                    {6,  2,  13, 18, 20},
                    {2,  5,  19, 17,
                                     13},
                    {4,  20, 18, 10, 15},
                    {18, 13, 17, 9,  10},
                    {17, 19, 3,  14,
                                     9},
                    {3,  7,  16, 1,  14},
                    {16, 8,  4,  15, 1}};

typedef vector<int> line;
typedef set<int> pointSet;
typedef vector<pair<int, int> > perm;
bool faceAdj[12][12];
alg turns;

struct vec3 {
    double a[3];

    vec3() {}

    vec3(double x, double b, double c) {
        a[0] = x;
        a[1] = b;
        a[2] = c;
    }

    double operator[](int i) const {
        return a[i];
    }

    double &operator[](int i) {
        return a[i];
    }

    vec3 operator-(vec3 v) {
        return vec3(a[0] - v[0], a[1] - v[1], a[2] - v[2]);
    }
};

vec3 cross(vec3 a, vec3 b) {
    return vec3(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

double dot(vec3 a, vec3 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}



void printAlg(alg Alg) {
    rep(i, 0, Alg.size()) cout << Alg[i] << " ";
    cout << endl;
}

//typedef tuple<int, int, int> corner;
//typedef pair<int, int> edge;
//vector<corner> cornersFace[12];
//vector<edge> edgesFace[12];
//map<corner, int> corners;
//map<edge, int> edges;
vector<tuple<double, double, double> > pieces;
map<tuple<int, int, int>, int> pieceMap;

tuple<int, int, int> double2Int(tuple<double, double, double> piece) {
    return make_tuple(int(XX * get<0>(piece)), int(XX * get<1>(piece)), int(XX * get<2>(piece)));
}

map<string, perm> perms;
string colors[] = {"orange", "red", "deepGreen", "white", "blue", "pink", "fleshColor", "shallowGreen", "grey",
                   "skyBlue", "purple", "yellow"};
alg notations{"DBL", "R", "F", "U", "BR", "DBR", "DR", "B", "D", "DL", "L", "BL"};
string suffix[] = {"'2", "'", "", "2"};
double angles[] = {acos(-1) * 0.8, acos(-1) * 0.4, -acos(-1) * 0.4, -acos(-1) * 0.8};

map<string, int> colorIndex;
int corners[12][12][12][3], edges[12][12][2], cornerRef[12][12];

map<line, set<tuple<int, int, int> > > cornersInLine;
map<line, set<pair<int, int> > > edgesInLine;
set<line> legalLines;
map<line, map<line, tuple<int, int, int> > > precedents;
vector<line> sepLines;



alg inverse(alg a) {
    alg ret;
    for (int i = a.size() - 1; i >= 0; i--) {
        if (a[i][a[i].length() - 1] == '\'') ret.emplace_back(a[i].substr(0, a[i].length() - 1));
        else {
            if (a[i][a[i].length() - 2] == '\'') ret.emplace_back(a[i].substr(0, a[i].length() - 2) + "2");
            else if (a[i][a[i].length() - 1] == '2') ret.emplace_back(a[i].substr(0, a[i].length() - 1) + "\'2");
            else ret.emplace_back(a[i] + '\'');
        }
    }
    return ret;
}

alg lazyA{"F", "R", "U", "R'", "U'", "F'"}, lazyAPrime = inverse(lazyA);

alg lazyB{"R'", "U'2", "R", "U", "R'", "U", "R"}, lazyBPrime = inverse(lazyB);

alg lazyC{"R'", "U'2", "F'", "U", "F", "R", "L", "F", "U", "F'", "U'", "L'", "R'", "U", "R"}, lazyCPrime = inverse(lazyC);

alg lazyD{"R'2", "F2", "R'", "DBR'", "R", "F'2", "R'", "DBR", "R'2"}, lazyDPrime = inverse(lazyD);

void print(alg a, ofstream &s) {
    rep(i, 0, a.size()) {
//        if (a[i].substr(0, 7) == "insertA") {
//            s << "R' U" << a[i].substr(7, a[i].size() - 7) << " R ";
//        } else if (a[i].substr(0, 7) == "insertB") {
//            s << "R U" << a[i].substr(7, a[i].size() - 7) << " R' ";
//        } else if (a[i] == "flip") {
//            s << "R U R' F R' F' R ";
//        } else
        if (a[i] == "lazyA") print(lazyA, s);
        else if (a[i] == "lazyB") print(lazyB, s);
        else if (a[i] == "lazyC") print(lazyC, s);
        else if (a[i] == "lazyD") print(lazyD, s);
//        else if (a[i] == "parityA") print(parityA, s);
//        else if (a[i] == "parityB") print(parityB, s);
        else if (a[i] == "lazyA'") print(lazyAPrime, s);
        else if (a[i] == "lazyB'") print(lazyBPrime, s);
        else if (a[i] == "lazyC'") print(lazyCPrime, s);
        else if (a[i] == "lazyD'") print(lazyDPrime, s);
//        else if (a[i] == "parityA'") print(parityA, s);
//        else if (a[i] == "parityB'") print(parityB, s);
        else s << a[i] << " ";
    }
    s << endl;
}


void printPerm(perm Perm) {
    for (auto const &p: Perm) cout << p.first << " " << p.second << endl;
    cout << endl;
}


void appoint(double x[3], double b, double angle, string notation) {
    vector<tuple<double, double, double> > rotated;
    double center[3];
    int cnt = 0;
    rep(i, 0, pieces.size()) {
        double p[3] = {get<0>(pieces[i]), get<1>(pieces[i]), get<2>(pieces[i])};
        double dot = 0;

        rep(k, 0, 3) dot += x[k] * p[k];
//        cout << dot << endl;
        if (dot > b) {
            rotated.emplace_back(pieces[i]);
            rep(k, 0, 3) center[k] += p[k];
            cnt++;
        }
    }
    rep(k, 0, 3) center[k] /= cnt;

    perm ans;

    rep(i, 0, rotated.size()) {
        double p[3] = {get<0>(rotated[i]), get<1>(rotated[i]), get<2>(rotated[i])};
        double dot = 0, x2 = 0, p1[3];
        rep(k, 0, 3) {
            dot += x[k] * p[k];
            x2 += x[k] * x[k];
        }
        rep(k, 0, 3) p1[k] = p[k] - x[k] * dot / x2;
        double p2[3]{x[1] * p1[2] - x[2] * p1[1], x[2] * p1[0] - x[0] * p1[2], x[0] * p1[1] - x[1] * p1[0]};
        double p22 = 0, p12 = 0;
        rep(k, 0, 3) {
            p22 += p2[k] * p2[k];
            p12 += p1[k] * p1[k];
        }
        double newp[3];
        rep(k, 0, 3) newp[k] = cos(angle) * p1[k] + x[k] * dot / x2;
        rep(k, 0, 3) newp[k] += sin(angle) * p2[k] * sqrt(p12 / p22);
        ans.emplace_back(mp(pieceMap[double2Int(rotated[i])],
                            pieceMap[make_tuple(int(XX * newp[0]), int(XX * newp[1]), int(XX * newp[2]))]));
    }

    perms[notation] = ans;
}

state solvedState() {
    state State;
    rep(i, 0, 132) State.a[i] = i / 11;
    return State;
}

state apply(state State, perm p) {
    state ans = State;
    rep(i, 0, p.size()) ans.a[p[i].second] = State.a[p[i].first];
    return ans;
}

state apply(state State, vector<string> ps) {
    state ans = State;
    rep(i, 0, ps.size()) ans = apply(ans, perms[ps[i]]);
    return ans;
}

void printState(state State) {
    cout << colors[State.a[36]] << endl;
    cout << colors[State.a[33 + 8]] << " " << colors[State.a[33 + 7]] << endl;
    cout << colors[State.a[33 + 4]] << " " << colors[State.a[33 + 0]] << " " << colors[State.a[33 + 2]] << endl;
    cout << colors[State.a[33 + 9]] << " " << colors[State.a[33 + 6]] << endl;
    cout << colors[State.a[33 + 5]] << " " << colors[State.a[33 + 10]] << " " << colors[State.a[33 + 1]] << endl;

}

void printLine(line l) {
    rep(i, 0, l.size()) cout << l[i] << " ";
    cout << endl;
}


vector<line> findAllSepLine() {
    vector<line> answer;
    queue<line> q;
    q.push(line{});
    while (!q.empty()) {
        auto troden = q.front();
//        if (troden.size() >= 4 && troden[0] == troden[troden.size() - 1]) {
//            auto p = troden;
//            p.pop_back();
//            answer.push_back(p);
////            rep(i, 0, troden.size()) cout << troden[i] << " ";
////            cout << endl;
//        }
        pointSet trodenSet{};
        rep(i, 0, troden.size()) {
            trodenSet.insert(troden[i]);
        }
        if (troden.size() < 8888)
            rep(i, 0, 12) {
                if (!troden.empty() && i < troden[0]) continue;
                if (!troden.empty() && !faceAdj[troden[troden.size() - 1]][i]) continue;
                if (troden.size() != 1 && trodenSet.count(i)) {
                    if (troden[0] == i) answer.push_back(troden);
//                    printLine(troden);
                    continue;
                }

                troden.push_back(i);
                q.push(troden);
                troden.pop_back();
            }
        q.pop();
    }
    return answer;
}

line rotate(line l) {
    line ret;
    rep(i, 1, l.size()) ret.emplace_back(l[i]);
    rep(i, 0, 1) ret.emplace_back(l[i]);
    return ret;
}

line reduce(line l) {
    int count[12]{0};
    rep(i, 0, l.size()) count[l[i]]++;
    rep(i, 0, l.size()) if (count[l[i]] > 1) {
            line tmp;
            rep(j, i, l.size()) tmp.emplace_back(l[j]);
            rep(j, 0, i) tmp.emplace_back(l[j]);
            l = tmp;
            break;
        }
    line ans;
    rep(i, 0, l.size()) {
        if (ans.size() <= 1 || l[i] != ans[ans.size() - 2]) ans.emplace_back(l[i]);
        else ans.pop_back();
    }
    if (ans.size() >= 2 && ans[ans.size() - 2] == ans[0]) {
        ans.pop_back();
        ans.pop_back();
    }
    int min = 999, pos;
    rep(i, 0, ans.size()) if (ans[i] < min) {
            min = ans[i];
            pos = i;
        }
    line ret;
    rep(i, pos, ans.size()) ret.emplace_back(ans[i]);
    rep(i, 0, pos) ret.emplace_back(ans[i]);
    return ret;
}

pair<int, int> pair_sorted(int x, int y) {
    if (x < y) return mp(x, y); else return mp(y, x);
}

tuple<int, int, int> tuple_sorted(int x, int y, int z) {
    int a[3]{x, y, z};
    sort(a, a + 3);
    return make_tuple(a[0], a[1], a[2]);
};

void find(line l) {

    if (edgesInLine.count(l)) return;
    if (l.size() == 2) {
        edgesInLine[l] = set<pair<int, int> >{pair_sorted(l[0], l[1])};
        cornersInLine[l] = set<tuple<int, int, int> >{};
        return;
    }
    auto pre = precedents[l].begin();
    auto preLine = pre->first;
    auto tp = pre->second;
    int t[3]{get<0>(tp), get<1>(tp), get<2>(tp)};
    edgesInLine[l] = set<pair<int, int> >{pair_sorted(t[0], t[1]), pair_sorted(t[1], t[2]), pair_sorted(t[2], t[0])};
    cornersInLine[l] = set<tuple<int, int, int> >{tuple_sorted(t[0], t[1], t[2])};
    if (edgesInLine.count(preLine) == 0) find(preLine);
    edgesInLine[l].insert(edgesInLine[preLine].begin(), edgesInLine[preLine].end());
    cornersInLine[l].insert(cornersInLine[preLine].begin(), cornersInLine[preLine].end());

}


perm composite(perm p1, perm p2) {

    rep(i, 0, p1.size()) swap(p1[i].first, p1[i].second);
    rep(i, 0, p2.size()) swap(p2[i].first, p2[i].second);
    map<int, int> m;
    perm p;
    rep(i, 0, p1.size()) {
        m[p1[i].first] = p1[i].second;
    }
    rep(i, 0, p2.size()) {
        if (m.count(p2[i].second)) {
            p.emplace_back(mp(p2[i].first, m[p2[i].second]));
            m.erase(p2[i].second);
        } else p.emplace_back(mp(p2[i].first, p2[i].second));
    }
    for (const auto &item : m) {
        p.emplace_back(mp(item.first, item.second));
    }
    rep(i, 0, p.size()) swap(p[i].first, p[i].second);

    return p;
}

perm permFromStr(vector<string> s) {
    perm ret;
    rep(i, 0, s.size()) {
        ret = composite(ret, perms[s[i]]);
    }
    return ret;
}

void init() {
    rep(i, 0, 12) colorIndex[colors[i]] = i;
    rep(i, 0, 12) rep(j, 0, 5) faces[i][j]--;
    int cnt = 0;
    rep(i, 0, 20) rep(j, 0, 12) rep(k, 0, 12) rep(jj, 0, 5) rep(kk, 0, 5) if (j != k && faces[j][jj] == faces[k][kk]) {
                            faceAdj[j][k] = true;
                        }

    rep(i, 0, 12) {
        double center[3] = {0};
        rep(j, 0, 5) rep(k, 0, 3) center[k] += vertex[faces[i][j]][k];
        rep(k, 0, 3) center[k] /= 5;
        pieces.emplace_back(make_tuple(center[0], center[1], center[2]));
        rep(j, 0, 5) pieces.emplace_back(make_tuple(vertex[faces[i][j]][0] * 0.9 + center[0] * 0.1,
                                                    vertex[faces[i][j]][1] * 0.9 + center[1] * 0.1,
                                                    vertex[faces[i][j]][2] * 0.9 + center[2] * 0.1));
        double mid[3];
        rep(j, 0, 5) {
            rep(k, 0, 3) mid[k] = (vertex[faces[i][j]][k] + vertex[faces[i][(j + 1) % 5]][k]) / 2;

            pieces.emplace_back(make_tuple(mid[0] * 0.9 + center[0] * 0.1,
                                           mid[1] * 0.9 + center[1] * 0.1,
                                           mid[2] * 0.9 + center[2] * 0.1));
        }
    }
    rep(i, 0, pieces.size()) {
        auto piece = pieces[i];
//        cout << "{" << get<0>(piece) << ", " << get<1>(piece) << ", " << get<2>(piece) << "}, \n";
        pieceMap[double2Int(piece)] = i;
    }


    rep(i, 0, 12) {
        double vr[3], vl[3];
        rep(k, 0, 3) vr[k] = vertex[faces[i][2]][k] - vertex[faces[i][1]][k];
        rep(k, 0, 3) vl[k] = vertex[faces[i][0]][k] - vertex[faces[i][1]][k];
        double n[3]{vr[1] * vl[2] - vr[2] * vl[1], vr[2] * vl[0] - vr[0] * vl[2], vr[0] * vl[1] - vr[1] * vl[0]};
        double b = 0;
        rep(k, 0, 3) b += vertex[faces[i][0]][k] * n[k];

//        rep(j, 1, 5) {
//            double b1 = 0;
//            rep(k, 0, 3) b1 += vertex[faces[i][j]][k] * n[k];
//            if (b1 != b)
//                cout << "!!" << endl;
//        }
        b -= 0.1;

        rep(j, 0, 4) appoint(n, b, angles[j], notations[i] + suffix[j]);
    }

//    printPerm(perms["DR"]);
//    printPerm(perms["DR'"]);

    int Rppface = 10;
    double vr[3], vl[3];
    rep(k, 0, 3) vr[k] = vertex[faces[Rppface][2]][k] - vertex[faces[Rppface][1]][k];
    rep(k, 0, 3) vl[k] = vertex[faces[Rppface][0]][k] - vertex[faces[Rppface][1]][k];
    double n[3]{vr[1] * vl[2] - vr[2] * vl[1], vr[2] * vl[0] - vr[0] * vl[2], vr[0] * vl[1] - vr[1] * vl[0]};
    rep(i, 0, 3) n[i] = -n[i];
    double b = 0;
    rep(k, 0, 3) b += vertex[faces[Rppface][0]][k] * n[k];
    b += 0.1;
    appoint(n, b, angles[3], "R++");
    appoint(n, b, angles[0], "R--");

    int Dppface = 3;
    rep(k, 0, 3) vr[k] = vertex[faces[Dppface][2]][k] - vertex[faces[Dppface][1]][k];
    rep(k, 0, 3) vl[k] = vertex[faces[Dppface][0]][k] - vertex[faces[Dppface][1]][k];
    double nn[3]{vr[1] * vl[2] - vr[2] * vl[1], vr[2] * vl[0] - vr[0] * vl[2], vr[0] * vl[1] - vr[1] * vl[0]};
    rep(i, 0, 3) n[i] = -nn[i];
    b = 0;
    rep(k, 0, 3) b += vertex[faces[Dppface][0]][k] * n[k];
    b += 0.1;
    appoint(n, b, angles[3], "D++");
    appoint(n, b, angles[0], "D--");

//    cout << perms["D++"].size() << endl;

    rep(i, 0, notations.size()) {
        turns.emplace_back(notations[i]);
        turns.emplace_back(notations[i] + "'");
        turns.emplace_back(notations[i] + "2");
        turns.emplace_back(notations[i] + "'2");
    }
    bool mark[132]{0};
    rep(k, 0, 20) {
        vector<int> cornerIndex;
        rep(i, 0, 132) {
            double dx[3] = {get<0>(pieces[i]) - vertex[k][0], get<1>(pieces[i]) - vertex[k][1],
                            get<2>(pieces[i]) - vertex[k][2]};
            if (dx[0] * dx[0] + dx[1] * dx[1] + dx[2] * dx[2] < 0.1) cornerIndex.emplace_back(i);
        }
//        cout << cornerIndex.size() << endl;
        vec3 p[3];
        rep(i, 0, 3) p[i] = vec3(get<0>(pieces[cornerIndex[i]]), get<1>(pieces[cornerIndex[i]]),
                                 get<2>(pieces[cornerIndex[i]]));
        auto p12 = p[2] - p[1], p10 = p[0] - p[1], cr = cross(p12, p10);
        vec3 center(vertex[k][0], vertex[k][1], vertex[k][2]);
        double dir = dot(cr, center - p[1]);
        int x = cornerIndex[0] / 11, y = cornerIndex[1] / 11, z = cornerIndex[2] / 11;
        if (dir < 0) swap(y, z);
        cornerRef[x][y] = z;
        cornerRef[y][z] = x;
        cornerRef[z][x] = y;
//        cout << x << " " << y << " " << z << endl;
        int xyz[3] = {x, y, z};
        sort(xyz, xyz + 3);
        rep(i, 0, 3) {
            corners[xyz[0]][xyz[1]][xyz[2]][i] = cornerIndex[i];
            mark[cornerIndex[i]] = 1;
        }
    }
    rep(i, 0, 132) rep(j, i + 1, 132) {
            if (mark[i] || mark[j]) continue;
            vec3 pi{get<0>(pieces[i]), get<1>(pieces[i]), get<2>(pieces[i])};
            vec3 pj{get<0>(pieces[j]), get<1>(pieces[j]), get<2>(pieces[j])};
            vec3 dis = pi - pj;
            if (dis[0] * dis[0] + dis[1] * dis[1] + dis[2] * dis[2] < 0.1) {
                edges[i / 11][j / 11][0] = i;
                edges[i / 11][j / 11][1] = j;
//                cout << i << " " << j << endl;
            }
        }

    sepLines = findAllSepLine();
    rep(i, 0, sepLines.size()) {
        legalLines.insert(sepLines[i]);
    }

    cout << sepLines.size() << endl;


    rep(i, 0, sepLines.size()) {
        if (sepLines[i] == vector<int>{0, 7, 8}) {
            cout << "";
        }
        auto l = sepLines[i];
        rep(j, 0, l.size()) {
            int erosion = cornerRef[l[1]][l[0]];
            auto nl = l;
            nl.insert(nl.begin() + 1, erosion);
            nl = reduce(nl);
            if (legalLines.count(nl)) precedents[nl][sepLines[i]] = make_tuple(l[0], l[1], erosion);
            l = rotate(l);
        }
    }
//    rep(i, 0, 100) {
//        printLine(sepLines[i]);
//        cout << "---------" << endl;
//        rep(j, 0, precedents[sepLines[i]].size())
//            printLine(precedents[sepLines[i]][j]);
//    }
    rep(i, 0, sepLines.size())if (sepLines[i].size() > 2 && precedents[sepLines[i]].size() == 0)
            printLine(sepLines[i]);

//    rep(i, 0, 1000) {
//        cout << sepLines[i].size() << " " << precedents[sepLines[i]].size() << endl;
//    }

//    rep(i, 0, 100) {
//        cout << "------------\n";
//        printLine(sepLines[i]);
//        rep(j, 0, precedents[sepLines[i]].size()) printLine(precedents[sepLines[i]][j].second);
//    }

    rep(i, 0, sepLines.size()) find(sepLines[i]);

//    rep(i, 0, 100) {
//        cout << sepLines[i].size() << " " << edgesInLine[sepLines[i]].size() << " " << cornersInLine[sepLines[i]].size() << endl;
//    }
    perms["lazyA"] = permFromStr(lazyA);
    perms["lazyA'"] = permFromStr(lazyAPrime);

    perms["lazyB"] = permFromStr(lazyB);
    perms["lazyB'"] = permFromStr(lazyBPrime);

    perms["lazyC"] = permFromStr(lazyC);
    perms["lazyC'"] = permFromStr(lazyCPrime);

    perms["lazyD"] = permFromStr(lazyD);
    perms["lazyD'"] = permFromStr(lazyDPrime);

}


alg bd_bfs(state State1, state State2, vector<string> turns, int kTh) {
    if (State1 == State2) return alg{};
    queue<state> q1, q2;
    map<state, alg> record1, record2;
    q1.push(State1);
    q2.push(State2);
    record1[State1] = vector<string>{};
    record2[State2] = vector<string>{};

    for (int k = 1; k <= kTh; k++) {
//        cout << k << endl;
//        cout << q1.size() << " " << q2.size() << endl;
        while (!q1.empty()) {
            auto head = q1.front();
            if (record1[head].size() == k) break;
            for (auto const &turn: turns) {
                auto new1 = apply(head, perms[turn]);
                if (record1.count(new1) == 0) {
                    record1[new1] = record1[head];
                    record1[new1].emplace_back(turn);
                    q1.push(new1);
                }
            }
            q1.pop();
        }
        state best_new2;
        int best = 999;
        while (!q2.empty()) {
            auto head = q2.front();
            if (record2[head].size() == k) break;
            for (auto const &turn: turns) {
                auto new2 = apply(head, perms[turn]);
                if (record2.count(new2) == 0) {
                    record2[new2] = record2[head];
                    record2[new2].emplace_back(turn);
                    q2.push(new2);
                    if (record1.count(new2) != 0 && record1[new2].size() < best) {
                        best = record1[new2].size();
                        best_new2 = new2;
                    }
                }
            }
            q2.pop();
        }
        if (best != 999) {
            auto ans = record1[best_new2], inv = inverse(record2[best_new2]);
            ans.insert(ans.end(), inv.begin(), inv.end());
            return ans;
        }
    }
    return vector<string>{"too long"};
}

void printAll(state State) {
    rep(i, 0, 132) cout << State.a[i] << " ";
    cout << endl;
}

alg solveState(state State, line l) {
    state State1, State2;
    int nowColors[12];
    rep(i, 0, 12) nowColors[i] = State.a[i * 11];
    for (auto const &edge: edgesInLine[l]) {
        int id[2];
        rep(i, 0, 2) {
            id[i] = edges[edge.first][edge.second][i];
            State2.a[id[i]] = nowColors[id[i] / 11];
        }
        rep(i, 0, 12) rep(j, i + 1, 12) {
                int id1[2];
                rep(k, 0, 2) id1[k] = edges[i][j][k];
                if ((State.a[id1[0]] == nowColors[id[0] / 11] && State.a[id1[1]] == nowColors[id[1] / 11]) ||
                    (State.a[id1[1]] == nowColors[id[0] / 11] && State.a[id1[0]] == nowColors[id[1] / 11])) {
                    State1.a[id1[0]] = State.a[id1[0]];
                    State1.a[id1[1]] = State.a[id1[1]];
                }
            }
    }
    for (auto const &corner: cornersInLine[l]) {
        int id[3];
        rep(i, 0, 3) {
            id[i] = corners[get<0>(corner)][get<1>(corner)][get<2>(corner)][i];
            State2.a[id[i]] = nowColors[id[i] / 11];
        }
        rep(i, 0, 12) rep(j, i + 1, 12) rep(k, j + 1, 12) {
                    int id1[3];
                    rep(p, 0, 3) id1[p] = corners[i][j][k][p];
                    if (tuple_sorted(State.a[id1[0]], State.a[id1[1]], State.a[id1[2]]) ==
                        tuple_sorted(nowColors[id[0] / 11], nowColors[id[1] / 11], nowColors[id[2] / 11])) {
                        rep(p, 0, 3) State1.a[id1[p]] = State.a[id1[p]];

                    }
                }
    }
//    printAll(State1);
//    printAll(State2);
    auto al = bd_bfs(State1, State2, turns, 4);
//    printAlg(al);
    return al;
}

map<line, alg> allSol;

state globalState;

alg solve(line l) {
    if (allSol.count(l)) return allSol[l];
    int shortestLen = 999;
    alg shortestAlg;
    if (precedents[l].size())
        for (auto const &pre_: precedents[l]) {
            auto pre = pre_.first;
            alg alPre = solve(pre);

            if (alPre.size() && alPre[0] == "too long" || alPre.size() >= shortestLen) continue;
            state State = apply(globalState, alPre);
            printAlg(alPre);
            cout << cornersInLine[l].size() << endl;

            alg al = solveState(State, l);
            if (al.size() && al[0] == "too long") continue;
            if (alPre.size() + al.size() < shortestLen) {
                shortestLen = alPre.size() + al.size();
                shortestAlg = alPre;
                shortestAlg.insert(shortestAlg.end(), al.begin(), al.end());
                break;
            }
        }
    else {
        auto al = solveState(globalState, l);
        allSol[l] = al;
        return al;
    }
    allSol[l] = shortestAlg;
    return shortestAlg;
}


ofstream f("alg.txt");

void solveTop() {


    auto lazySet1 = alg{"U", "U2", "U'", "U'2", "lazyA", "lazyA'", "lazyB", "lazyB'"};

    state State1, State2;

    printAll(globalState);

    rep(i, 33, 44) State2.a[i] = globalState.a[33];
    rep(i, 0, 132) if (globalState.a[i] == globalState.a[33]) State1.a[i] = globalState.a[33];

    printAll(State1);
    printAll(State2);
    auto al = bd_bfs(State1, State2, lazySet1, 4);

    printAll(apply(State1, al));

    printAlg(al);
    print(al, f);

    auto lazySet2 = alg{"U", "U2", "U'", "U'2", "lazyC", "lazyC'", "lazyD", "lazyD'"};

    globalState = apply(globalState, al);

    rep(i, 0, 132) State2.a[i] = globalState.a[11 * (i / 11)];

    printAll(globalState);
    printAll(State2);

    al = bd_bfs(globalState, State2, lazySet2, 5);
    print(al, f);

}

int main() {
    srand(time(0));
    init();


//    cout << sepLines.size();
//    string test = "R++ D++ R-- D++ R++ D++ R-- D++ R++ D-- U' \
//    R++ D-- R-- D++ R-- D++ R++ D-- R-- D++ U \
//    R++ D++ R++ D++ R-- D++ R++ D++ R++ D++ U \
//    R-- D++ R++ D++ R-- D++ R++ D-- R-- D++ U \
//    R++ D-- R++ D++ R++ D-- R++ D-- R++ D++ U \
//    R++ D++ R-- D++ R-- D-- R-- D++ R-- D++ U \
//    R-- D-- R-- D++ R++ D-- R++ D-- R-- D-- U'";
    string test = "";
    rep(i, 0, 7){
        rep(j, 0, 10) {
            if (j % 2) test += "D"; else test += "R";
            if (rand() % 2) test +=  "++ "; else test += "-- ";
        }
        if (rand() % 2) test += "U "; else test += "U' ";
    }
    state State = solvedState();
    string turn;
    stringstream input(test);
    while (input >> turn) {
        State = apply(State, perms[turn]);
    }


    globalState = State;

    auto scrambled = ofstream("state.txt");

    scrambled << test << endl;

    rep(i, 0, sepLines.size()) {
        if (sepLines[i].size() == 5 && cornersInLine[sepLines[i]].size() == 15) {

            if (cornerRef[sepLines[i][1]][sepLines[i][0]] != 3) continue;
            auto al = solve(sepLines[i]);
            print(al, f);
            globalState = apply(globalState, al);
            break;
        }
    }

    solveTop();


}