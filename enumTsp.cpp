
/// ###
/// int tspSolver(void) を宣言して，その中に自分のアルゴリズムを書いてください．
/// 入出力データはグローバル変数でやりとりします．
/// 入力データは 都市数 n，都市座標の二次元配列 city[n][2] として入力されます．
/// 出力データは 順回路 tour[n], 順回路長 length で出力しますが，
/// length は最後に計算し直されるので，あまり出力する意味はありません．

/// 使用するグローバル変数等．必須項目
#define MAX 20000
#define INF 99999999
#define TEMP_INIT 100
#define TEMP_FACT 0.99
#define TEMP_FIN 0.1
#define MAX_ITR 200
#define EXEC_TIME 10
#define NGH_SIZE 5

#include <bits/stdc++.h>
#include <chrono>
using namespace std;

extern int n, length, tour[MAX];
extern float city[MAX][2];

/// 順回路表示のための関数
/// tou[] に順回路の配列を，wai にウェイト時間(ミリ秒)，colorに色 0~3を指定．
extern void showTour(int *tou, int wai, int color);
/// color[] に配列を指定して，枝ごとに色を変えられる順回路表示．
extern void showCTour(int *tou, int wai, int *color);
/// 文字列を表示ウィンドウの上部バーに表示．数字等が表示したい場合 sprintf
/// 等と組み合わせる．
extern void showString(char *str);
/// leng を暫定解として表示(標準エラー出力)
extern void showLength(int leng);
/// ###

int tr[MAX];
int head[MAX], adj[MAX];
double temp;
int itr;

/// 距離の計算はこの関数と同等の方法で行う．
/// 必ずしもこの関数を残しておく必要は無い．
int Dis(int i, int j) {
  float xd, yd;

  xd = city[i][0] - city[j][0];
  yd = city[i][1] - city[j][1];
  return (int)(sqrt(xd * xd + yd * yd) + .5);
}

/// 以下 int tspSolver(void) が必要なこと以外は自由に作る．
///
int cost_evaluate() {
  int j;
  int sum;

  sum = Dis(tr[n - 1], tr[0]);
  for (j = 0; j < n - 1; j++) {
    sum += Dis(tr[j], tr[j + 1]);
  }

  return sum;
}

void swap_arr(int *a, int i, int j) {
  int tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

void init_array() {
  for (int i = 0; i < n; i++) {
    tr[i] = i;
    tour[i] = i;
  }
  for (int i = 0; i < n; i++) {
    int j = rand() % n;
    swap_arr(tr, i, j);
    swap_arr(tour, i, j);
  }
}

int swap_cost(int i, int j) {
  return - Dis(tour[i%n], tour[(i+1)%n]) \
         - Dis(tour[j%n], tour[(j+1)%n]) \
         + Dis(tour[i%n], tour[j%n]) \
         + Dis(tour[(i+1)%n], tour[(j+1)%n]);
}

double getTemplature(double r) {
  return TEMP_FACT * r;
}

double calc_probability(double c, double t) {
  return exp(-c / t);
}

void reverse(int i, int j) {
  for (int k = 0; k < (j-i) / 2; k++) {
    swap_arr(tour, (i+1+k)%n, (j-k)%n);
    swap_arr(tr, (i+1+k)%n, (j-k)%n);
  }
}

void log_tour() {
  printf("temp: %10f\n", temp);
  for (int i = 0; i < n; i++) {
    printf("%d ", tr[i]);
  }
  printf("\n");
  printf("cost: %d\n", cost_evaluate());
  showTour(tr, 10, 1);
}

void log_prob(int cost, double prob, double rand) {
  printf("cost         %d\n", cost);
  printf("probability: %.10f\n", prob);
  printf("rand:        %.10f\n", rand);
}

int update(int mi) {
  if (cost_evaluate() < mi) {
    mi = cost_evaluate();
    for (int i = 0; i < n; i++) {
      tour[i] = tr[i];
    }
  }

  return mi;
}

using clck = chrono::system_clock::time_point;

bool isInTime(clck st, clck nw) {
  return chrono::duration_cast<chrono::seconds>(nw - st).count() < EXEC_TIME;
}

void tsp(int s) {
  bool flag = true;
  int mi_cost = INF, cnt = 0;
  auto start_t = chrono::system_clock::now();
  while (isInTime(start_t, chrono::system_clock::now()) && flag && temp > TEMP_FIN) {
    for (int itr = 0; itr < MAX_ITR; itr++) {
      flag = false;
      for (int i = s; i < s+n; i++) {
        for (int j = i+2; j < i+n-1; j++) {
          if (++cnt < n * NGH_SIZE) {
            cnt = 1;
          }
          int cost = swap_cost(i, j);
          if (cost < 0) {
            reverse(i, j);
            s = (i+1) % n;
            flag = true;
            break;
          } else {
            double prob = calc_probability((double)cost, temp);
            double rd = (double)rand() / RAND_MAX;
            if (rd < prob) {
              reverse(i, j);
              s = (i+1) % n;
              flag = true;
              break;
            }
          }
          mi_cost = update(mi_cost);
        }
        if (flag) break;
      }
    }
    temp = getTemplature(temp);
//    log_tour();
  }
}

int tspSolver(void) {
  init_array();
  temp = TEMP_INIT;

  chrono::system_clock::time_point start, end; 
  start = chrono::system_clock::now();
  tsp(0);
  end = chrono::system_clock::now();
  printf("time: %lf[ms]\n", static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count())); 

  return 1;
}
