=======================  THAM KHẢO GROK 3  ======================================
=======================  THAM KHẢO GROK 3  ======================================
=======================  THAM KHẢO GROK 3  ======================================

3. Thiết kế Class và Cấu trúc liên kết
Dưới đây là các class chính cho game, được thiết kế theo hướng OOP (Object-Oriented Programming) với tính kế thừa và module hóa.

Class Diagram (Tổng quan):
text

Collapse

Wrap

Copy
Game
├── Map
├── TowerManager
│   ├── Tower (Abstract)
│   │   ├── ArcherTower
│   │   ├── CannonTower
│   │   └── MagicTower
├── EnemyManager
│   ├── Enemy (Abstract)
│   │   ├── FastEnemy
│   │   ├── ArmoredEnemy
│   │   └── FlyingEnemy
├── BulletManager
│   └── Bullet
└── UI
Chi tiết các Class:
1. Game (Quản lý toàn bộ trò chơi)
Vai trò: Điều phối game loop, quản lý các manager, và xử lý trạng thái game.
Thuộc tính:
SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font.
Map map, TowerManager towerMgr, EnemyManager enemyMgr, BulletManager bulletMgr, UI ui.
bool running, int castleHP, int gold, int currentWave.
Phương thức:
init(): Khởi tạo SDL và các thành phần.
run(): Chạy game loop (handleEvents, update, render).
spawnWave(): Sinh wave kẻ thù mới.
2. Map (Bản đồ)
Vai trò: Quản lý lưới bản đồ, đường đi của kẻ thù, và hiển thị.
Thuộc tính:
vector<vector<Tile>> tiles: Lưới bản đồ (Tile chứa texture và thuộc tính như "đường" hay "đất").
vector<SDL_Point> path: Đường đi cố định cho kẻ thù.
Phương thức:
loadMap(string filename, SDL_Renderer* renderer): Tải bản đồ từ file.
render(SDL_Renderer* renderer): Vẽ bản đồ.
3. TowerManager (Quản lý tháp)
Vai trò: Quản lý danh sách tháp và tương tác với kẻ thù.
Thuộc tính:
vector<unique_ptr<Tower>> towers: Danh sách tháp (dùng smart pointer để tự động quản lý bộ nhớ).
Phương thức:
addTower(float x, float y, TowerType type): Thêm tháp mới.
update(Uint32 dt, EnemyManager& enemies): Cập nhật logic tháp (tìm mục tiêu, bắn).
4. Tower (Lớp trừu tượng cho tháp)
Vai trò: Định nghĩa hành vi chung của tháp.
Thuộc tính:
SDL_Rect rect, float range, float damage, Uint32 cooldown, Uint32 lastShot.
Phương thức:
virtual update(Uint32 dt, EnemyManager& enemies): Tìm mục tiêu và bắn (pure virtual).
virtual render(SDL_Renderer* renderer): Vẽ tháp và phạm vi.
Các lớp con:
ArcherTower: Bắn nhanh, sát thương thấp.
CannonTower: Bắn chậm, sát thương cao, có splash damage.
MagicTower: Thêm hiệu ứng (làm chậm, đốt cháy).
5. EnemyManager (Quản lý kẻ thù)
Vai trò: Quản lý danh sách kẻ thù và cập nhật vị trí.
Thuộc tính:
vector<unique_ptr<Enemy>> enemies.
Phương thức:
spawnEnemy(EnemyType type, SDL_Point spawnPoint): Sinh kẻ thù.
update(Uint32 dt, Map& map): Di chuyển kẻ thù theo path.
6. Enemy (Lớp trừu tượng cho kẻ thù)
Vai trò: Định nghĩa hành vi chung của kẻ thù.
Thuộc tính:
SDL_Rect rect, float speed, int hp, int pathIndex (chỉ số điểm trên path).
Phương thức:
virtual update(Uint32 dt, Map& map): Di chuyển theo path.
virtual render(SDL_Renderer* renderer): Vẽ kẻ thù.
Các lớp con:
FastEnemy: Tốc độ cao, HP thấp.
ArmoredEnemy: HP cao, tốc độ chậm.
FlyingEnemy: Bỏ qua path, bay thẳng đến lâu đài.
7. BulletManager (Quản lý đạn)
Vai trò: Quản lý danh sách đạn và xử lý va chạm.
Thuộc tính:
vector<Bullet> bullets.
Phương thức:
addBullet(float x, float y, Enemy* target, float speed, float damage): Thêm đạn.
update(Uint32 dt, EnemyManager& enemies): Cập nhật vị trí và kiểm tra va chạm.
8. Bullet (Đạn)
Vai trò: Đại diện cho đạn bắn từ tháp.
Thuộc tính:
SDL_Rect rect, float speed, float damage, Enemy* target.
Phương thức:
update(Uint32 dt): Di chuyển về phía mục tiêu.
render(SDL_Renderer* renderer): Vẽ đạn.
checkCollision(EnemyManager& enemies): Kiểm tra va chạm.
9. UI (Giao diện người dùng)
Vai trò: Hiển thị thông tin game (HP, vàng, wave).
Thuộc tính:
SDL_Texture* uiTexture, TTF_Font* font.
Phương thức:
render(SDL_Renderer* renderer, int castleHP, int gold, int wave): Vẽ UI.
Cấu trúc liên kết:
Game là trung tâm, điều khiển các manager (TowerManager, EnemyManager, BulletManager) và Map.
TowerManager tương tác với EnemyManager để tìm mục tiêu cho tháp.
BulletManager nhận đạn từ Tower và kiểm tra va chạm với EnemyManager.
Map cung cấp đường đi cho Enemy và nền cho giao diện.
Cách hoạt động:
Khởi tạo: Game khởi tạo SDL, tải bản đồ, và chuẩn bị UI.
Game Loop:
handleEvents: Xử lý input (đặt tháp, thoát game).
update: Cập nhật từng manager (TowerManager, EnemyManager, BulletManager).
render: Vẽ Map, Enemy, Tower, Bullet, và UI.
Gameplay:
Wave bắt đầu → EnemyManager sinh kẻ thù.
Người chơi đặt tháp → TowerManager thêm tháp, tháp tự động bắn kẻ thù trong phạm vi.
Đạn trúng kẻ thù → HP kẻ thù giảm, nếu HP = 0 thì bị tiêu diệt, người chơi nhận vàng.



=======================  THAM KHẢO GROK 3  ======================================
=======================  THAM KHẢO GROK 3  ======================================
=======================  THAM KHẢO GROK 3  ======================================

4. Kế hoạch phát triển chi tiết
Sprint 1: Thiết lập nền tảng
Công việc:
Tạo lớp Game với SDL setup.
Viết lớp Map để tải và vẽ bản đồ từ file text (ví dụ: 0 = đất, 1 = đường).
Thêm FPS counter cơ bản.
Kết quả: Một cửa sổ game với bản đồ tĩnh.


Sprint 2: Cơ chế kẻ thù
Công việc:
Tạo Enemy và EnemyManager.
Thêm path cố định trong Map (danh sách điểm SDL_Point).
Sinh kẻ thù ngẫu nhiên và di chuyển theo path.
Kết quả: Kẻ thù di chuyển từ điểm spawn đến lâu đài.


Sprint 3: Cơ chế tháp và bắn
Công việc:
Tạo Tower (chỉ 1 loại ban đầu) và TowerManager.
Thêm Bullet và BulletManager.
Tháp tự bắn kẻ thù trong phạm vi khi nhấn chuột để đặt.
Kết quả: Tháp bắn được kẻ thù cơ bản.


Sprint 4: Tài nguyên và UI
Công việc:
Thêm UI để hiển thị HP lâu đài, vàng, và wave.
Tích hợp hệ thống vàng (kiếm từ tiêu diệt kẻ thù, dùng để đặt tháp).
Thêm điều kiện thua (HP lâu đài = 0).
Kết quả: Game có mục tiêu và tài nguyên cơ bản.


Sprint 5: Đa dạng hóa
Công việc:
Thêm 2 loại tháp mới (Cannon, Magic).
Thêm 2 loại kẻ thù mới (Armored, Flying).
Tối ưu va chạm và spawn wave.
Kết quả: Game có chiều sâu và chiến thuật hơn.


Sprint 6: Hoàn thiện
Công việc:
Thêm menu chính (Start, Quit).
Thêm âm thanh (bắn, kẻ thù chết).
Thêm hiệu ứng (nổ khi kẻ thù chết).
Kết quả: Game hoàn chỉnh, sẵn sàng chơi.




chia ra các wave
-> sau s giây sẽ xuất hiện wave địch thứ n
mỗi wave sẽ random số lượng địch còn sống là e
mỗi wave sẽ kéo dài trong s giây

càng lên lv, số lượng đặt tháp pháo sẽ giảm hoặc số lượng địch sẽ tăng

10 level

1.
3 wave, mỗi wave 5 - 7 enemy, mỗi wave 30 giây, nghỉ 10s
4 tháp