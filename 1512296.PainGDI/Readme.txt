- Thái Bá Long - 1512296 - thaibalong7@gmail.com - PainGDI
- Đã làm được: 
	1. Đường thẳng (line). Dùng hàm MoveToEx và LineTo.
	2. Hình chữ nhật (rectangle). Dùng hàm Rectangle. Nếu giữ phím Shift sẽ vẽ hình vuông (Square).
	3. Hình Ellipse. Dùng hàm Ellipse. Nếu giữ phím Shift sẽ vẽ hình tròn (Circle).
	Cho phép chọn loại hình cần vẽ (từ menu). Thể hiện được là đang chọn menu nào.
-Sự kiện chính: 
	- Khi mở chương trình hiện lên với mặc định là đang vẽ line. Phía dưới là thanh status gồm 3 ô (ô 0 thể
	hiện kiểu hình đang được chọn, ô 1 là tọa độ của chuột khi nhấn giữa chuột trái, ô 2 là tọa độ của chuột
	không bất kỳ).
	- Thanh menu có phần Type để chọn kiểu hình muốn vẽ. Có 3 loại là Line, Rectangle và Ellipse. Khi xổ xuống
	sẽ thấy được kiểu nào đang được chọn, mặc định là Line.
	- Nhấn giữ phím Shift, khi đang chọn Line thì không thay đổi gì cả, khi đang chọn Rectangle thì sẽ vẽ ra 
	hình vuông, Ellipse sẽ vẽ ra hình tròn. Thả Shift quay lại như các hình cũ. Khi nhấn giữ Shift thì ô 0 của
	thanh status cũng thay đổi thành Square hoặc Circle.
-Sự kiện phụ:
	- Màu nét vẽ không phải là màu đen (đã đổi màu).
	- Hình vẽ được chuyển thành trong suốt để các hình Rectangle và Ellipse không phủ mất các hình đã vẽ trước
	đó.
	- Đóng chương trình thì không có chuyện gì xảy ra cả, hình ảnh được vẽ không được lưu lại.
	- Khi vẽ nhiều hình hay một hình lớn thì màn hình vẽ bị nhấp nháy (chưa khắc phục được lỗi này).
Link demo Youtube: https://www.youtube.com/watch?v=2AaQnHeGVyM
Link Bitbucket: https://bitbucket.org/thaibalong7/windev/src/052d39e57e0de240e5ec4cc8aa10fdd2500aa9b0/1512296.PainGDI/?at=master