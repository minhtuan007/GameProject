!!! Dự án tự xây dựng và thiết kế, không được based trên dự án nào có sẵn. !!!
------------------------------------------------------------------------------------------------------
Tên Game: The Last Bastion

- MỤC TIÊU CHÍNH
Người chơi phải bảo vệ cổng thành (Nexus Core) trước các đợt tấn công liên tiếp của enemy.
Nếu cổng thành mất toàn bộ HP → thua.
Nếu ngăn chặn được toàn bộ enemy của tất cả các wave → thắng.

- CƠ CHẾ GAMEPLAY
Bản đồ và đường đi
Enemy xuất hiện từ điểm spawn.
Di chuyển theo đường cố định trên map → hướng đến cổng thành.
Mỗi level có layout khác nhau → buộc người chơi thay đổi chiến thuật đặt tháp.

- Hệ thống máu của cổng thành
Cổng thành (Nexus Core) có 100 HP.
Mỗi enemy nếu đi tới được cổng thành → tự phát nổ, gây 20 damage lên cổng thành.

- Các item xây dựng
Người chơi có thể mua và đặt 3 loại item cố định:

🔷 Base (Đế tháp)
Item bắt buộc để đặt các loại tháp.
Đặt Base tạo ra nền cho phép xây Tower lên.

🔷 Wooden Tower (Tháp gỗ)
Giá: 70 coins.
Công dụng: Tower bắn đạn vào enemy trong phạm vi hoạt động.
Sát thương: Mỗi viên đạn gây 20 damage.
Cooldown: Bắn mỗi 0.3 giây.

🔷 Ice Tower (Tháp băng)
Công dụng: Khi enemy đi vào vùng hoạt động của Ice Tower, tower bắn đạn băng.
Hiệu ứng: Enemy trúng đạn bị giảm 50% tốc độ di chuyển + giảm 5 HP.

- Enemy
Mỗi enemy có 100 HP.
Nếu bị tiêu diệt trước khi tới cổng → enemy biến mất, không gây damage.
Nếu tới cổng thành → tự phát nổ gây 20 damage rồi biến mất.

- CÁCH CHƠI
Khi wave bắt đầu, enemy lần lượt di chuyển theo đường đi.
Người chơi dùng coins để mua Base → đặt tại vị trí chiến lược.
Xây Tower (Wooden / Ice) trên Base để tiêu diệt hoặc làm chậm enemy.
Kết hợp giữa Wooden Tower gây damage chính và Ice Tower làm chậm để đạt hiệu quả tối đa.

- ĐẶC ĐIỂM ĐẶC TRƯNG
Chiến thuật nằm hoàn toàn ở tính toán vị trí đặt base + lựa chọn loại tower + kết hợp Wood & Ice hợp lý.
Cơ chế tower defense cổ điển, dễ chơi, nhưng đủ thử thách ở level cao.

Src: Tham Khảo ChatGPT.
------------------------------------------------------------------------------------------------------

                         NGUỒN THAM KHẢO
    1. Thư viện SDL2
    2. Chatbot: ChatGPT, Grok với mục đích tối ưu hoá cấu trúc và chương trình, chiếm 5% - 10% code, các tính năng khác được tự xây dựng, chiếm 90% code.
    3. Đồ hoạ: - kenney.nl
               - designer.microsoft.com/image-creator 
    

       