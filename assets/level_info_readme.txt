lv1
giới hạn 4 base.
10s -> 3->5 địch kéo dài 30 giây: 40s
giây 50s -> 5->7 địch, 30 giây : 1p20s


struct LEVEL_INFO{
    int limitBase; = 4
    int vectorNum = 2;
    vector<int> startWave; 10, 50
    vector<int> waveLength; = 30, 30
    vector<int> enemyNum; 5, 7
};

ví dụ lv1
100                 //EnemyHP
500                 //EnemySpeed
100                 // fortressHP
200                 // coin
20                 // prize
4                  // limitBase
2                  // số phần tử của các vector
10 50             // startWave
30 30             // waveLength
3 4              // enemyNum, =>max 2n