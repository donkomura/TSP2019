
/// ###
/// int tspSolver(void) を宣言して，その中に自分のアルゴリズムを書いてください．
/// 入出力データはグローバル変数でやりとりします．
/// 入力データは 都市数 n，都市座標の二次元配列 city[n][2] として入力されます．
/// 出力データは 順回路 tour[n], 順回路長 length で出力しますが，
/// length は最後に計算し直されるので，あまり出力する意味はありません．

/// 使用するグローバル変数等．必須項目
#define MAX 20000
#define INF 9999
#define MAX_ITR 400

#include <bits/stdc++.h>
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

void perm(int i) {
  int j, tmp;
  int cost;

  if (i < n - 2) {
    perm(i + 1);
    for (j = i + 1; j < n - 1; j++) {
      tmp = tr[i];
      tr[i] = tr[j];
      tr[j] = tmp;
      perm(i + 1);
      tmp = tr[i];
      tr[i] = tr[j];
      tr[j] = tmp;
    }
  } else {
    cost = cost_evaluate();
    if (cost < length) {
      length = cost;
      for (j = 0; j < n; j++)
        tour[j] = tr[j];

      /// テスト等のために順回路等の表示機能が使える．
      showLength(length);
      showString("KOUSHIN!");
      showTour(tr, 1000, 1);
      showString("TANSAKU");
    } else {
      showTour(tr, 10, 0);
    }
  }
}

void swap(int *a, int i, int j) {
  int tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

int swap_cost(int i, int j) {
  return Dis(tour[i%n], tour[(i+1)%n]) \
    + Dis(tour[j%n], tour[(j+1)%n]) \
    - Dis(tour[i%n], tour[j%n]) \
    - Dis(tour[(i+1)%n], tour[(j+1)%n]);
}

double getTemplature(double itr) {
  return 100 - pow(100, itr / MAX_ITR);
}

double search_probability(double c) {
  return exp(-temp/c) - 1;
}

void tsp() {
  int s = 0;
  bool flag = true;
  int mi_cost = INF;
  while (flag && itr++ < MAX_ITR) {
    flag = false;
    for (int i = s; i < s+n; i++) {
      for (int j = i+2; j < i+n-1; j++) {
        int cost = swap_cost(i, j);
        if (cost > 0) {
          for (int k = 0; k < (j-i) / 2; k++) {
            swap(tr, (i+1+k)%n, (j-k)%n);
          }
          s = (i+1)%n;
          flag = true;
          break;
        } else {
          double prob = search_probability((double)cost);
          if (((double)rand()/RAND_MAX) <= prob) {
            printf("probability: %.10f\n", prob);
            printf("rand: %.10f\n", (double)rand()/RAND_MAX);
            for (int k = 0; k < (j-i) / 2; k++) {
              swap(tr, (i+1+k)%n, (j-k)%n);
            }
            s = (i+1)%n;
            flag = true;
            break;
          }
        }
      }
      if (flag) break;
    }
    temp = getTemplature(itr);

    if (cost_evaluate() < mi_cost) {
      mi_cost = cost_evaluate();
      for (int i = 0; i < n; i++) {
        tour[i] = tr[i];
      }
    }
    
    printf("temp: %.10f\n", temp);
    printf("itr: %d\n", itr);
    for (int i = 0; i < n; i++) {
      printf("%d ", tr[i]);
    }
    printf("\n");
    cout << cost_evaluate() << endl;
    showTour(tr, 10, 0);
  }
}

int tspSolver(void) {
  for (int i = 0;i < n; i++) {
    tour[i] = i;
    tr[i] = i;
  }
  temp = 100;
  itr = 0;
  tsp();
  length = cost_evaluate();
  printf("length: %d\n", length);
  for (int i = 0; i < n; i++) {
    printf("%d ", tour[i]);
  }
  printf("\n");

  return 1;
}