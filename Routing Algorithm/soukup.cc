#include <cstdlib>
#include <stack>
#include <queue>
#include <climits>
#include <cstring>
#include <bits/stdc++.h>

using namespace std;
char chipArea[100][100];
int n, nblocks, bx, by, numtarget, sx, sy, desx, desy;
bool Sblock[10000];
int Spath[10000], dx[4] = { -1,0,1,0 }, dy[4] = { 0,-1,0,1 };  //this is used to move in 4 directions
int srcX[100], srcY[100], desX[100], desY[100];
int super_id;

int targetDistance(int curr, int target) {
    int dis;
    int tx = target / n, ty = target % n, cx = curr / n, cy = curr % n;
    dis = abs(tx - cx) + abs(ty - cy);  //calculating the absolute distance
    return dis;
}

int toDirectionDistance(int next, bool visit[], int target) {
    int dis = INT_MAX;
    int tx = target / n, ty = target % n, nx = next / n, ny = next % n;

    for (int i = 0; i < 4; i++) {
        int x = nx + dx[i], y = ny + dy[i];  //used to change the direction and jump to adjacent cells
        int id = x * n + y;
        if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false) {
            if (dis > abs(tx - x) + abs(ty - y)) {
                dis = abs(tx - x) + abs(ty - y);
                super_id = id;
            }
        }
    }
    return dis;
}

int neighbourDist(int curr, bool visit[], int des) {
    int x = curr / n, y = curr % n;
    bool flag = true;
    int d;

    if (toDirectionDistance(curr, visit, des) <= targetDistance(curr, des)) {
        flag = false;
        d = super_id;
    }
    return flag == true ? -1 : d;
}

bool SoukupAlgorithm(int x1, int y1, int x2, int y2) {   
    bool visit[n*n];
    memset(visit, false, (n * n) + 1);
    int src = x1 * n + y1, des = x2 * n + y2;
    stack<int>plist;
    queue<int>nlist;
    //cout << "in plist " << src << endl;
    plist.push(src);
    Spath[src] = -1;
    visit[src] = true;

    while (!plist.empty()) {
        int pid = plist.top();

        if (pid == des) {
            return true;
        }

        if (toDirectionDistance(pid, visit, des) <= targetDistance(pid, des)) {
            int id = super_id;
            plist.push(id);
            visit[id] = true;
            Spath[id] = pid;
            if (id == des) {
                return true;
            }

            while (neighbourDist(id, visit, des) >= 0) {
                int new_id = neighbourDist(id, visit, des);
                plist.push(new_id);
                visit[new_id] = true;
                Spath[new_id] = id;
                if (new_id == des) {
                    return true;
                }
                id = new_id;
            }
        }
        while (!plist.empty()) {
            pid = plist.top();
            int tx = pid / n, ty = pid % n;
            for (int i = 0; i < 4; i++) {
                int x = tx + dx[i], y = ty + dy[i];
                int id = x * n + y;
                if ((x >= 0 && x < n && y >= 0 && y < n) && visit[id] == false && Sblock[id] == false) {
                    nlist.push(id);
                    visit[id] = true;
                    Spath[id] = pid;
                }
            }
            plist.pop();
        }
        while (!nlist.empty()) {
            plist.push(nlist.front());
            nlist.pop();
        }
    }
    return false;
}

int main()
{
    ifstream in("input.txt");
    ofstream out2("soukup.txt");
    in >> n;
    in >> nblocks;
    cout << "n" << n << endl;
    cout << "nblocks" << nblocks << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            chipArea[i][j] = '0';
        }
    }
    memset(Sblock, false, (n * n) + 1);  
    for (int i = 0; i < nblocks; i++) {
        in >> bx >> by;
        chipArea[bx][by] = '#';
        Sblock[bx * n + by] = true;
    }
    bool ans[2][100];    
    in >> numtarget;
    cout << "numtarget" << numtarget << endl;
    for (int i = 0; i < numtarget; i++) {
        in >> sx >> sy >> desx >> desy;
        srcX[i] = sx;
        srcY[i] = sy;
        desX[i] = desx;
        desY[i] = desy;
        chipArea[sx][sy] = 'S';
        chipArea[desx][desy] = 'T';
    }
    //input reading successfully 
    out2 << "Soukup Algorithm : " << endl;
    int i = 0;
    while (i < numtarget) {
        sx = srcX[i];
        sy = srcY[i];
        desx = desX[i];
        desy = desY[i];
        if (SoukupAlgorithm(sx, sy, desx, desy) == true) {    //printing of the output files
            int id = desx * n + desy;
            Sblock[id] = true;
            Sblock[sx * n + sy] = true;           
            while (Spath[id] != -1) {
                chipArea[Spath[id] / n][Spath[id] % n] = '*';
                Sblock[Spath[id]] = true;
                id = Spath[id];
            }
            chipArea[sx][sy] = 'S';
            ans[1][i] = true;           
            if (ans[1][i] == true) {
                out2 << "Net (" << sx << "," << sy << ")->(" << desx << "," << desy << ") can be routed." << endl;
            }            
        }
        else {      //if there is no route
            chipArea[sx][sy] = 's';
            chipArea[desx][desy] = 't';
            ans[1][i] = false;
            for (int j = 0; j < numtarget; j++) {
                if (ans[1][j] == false) {
                    out2 << "Net (" << sx << "," << sy << ")->(" << desx << "," << desy << ") cannot be routed." << endl;
                }
            }
            out2 << endl;
        }
        i++;
    } 
    for (int i = 0; i < n; i++) {	
        for (int j = 0; j < n; j++) out2 << chipArea[i][j] << " ";
        out2 << endl;
    }
    out2 << endl;
    return 0;
}

