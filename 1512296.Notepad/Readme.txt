- Thái Bá Long - 1512296 - thaibalong7@gmail.com
- Đã làm được: 
	1. Tạo ra giao diện có một edit control để nhập liệu.
	2. Lưu lại dưới dạng file text.
	3. Mở một tập tin, đọc nội dung và hiển thị để chỉnh sửa tiếp.
	4. Hỗ trợ 3 thao tác Cut - Copy - Paste.
-Sự kiện chính: 
	Code cứng tên tập tin, khi mở thì file được mở là "fileIn.txt", khi lưu thì tên file mặc định 
	 là "fileOut.txt".
	Các file .txt sẽ được lưu ở thư mục cùng cấp với tập tin thực thi (.exe).
	Khi mở file thực thi, edit control trống hiện lên để nhập liệu.
	Khi chọn File/Open, một thông báo hiện lên hỏi phần text đang được chỉnh sửa trên edit control
	 đã được lưu chưa, chọn Cancel để hủy lệnh Open để lưu text cũ, chọn OK để file "fileIn.txt"
	 được mở, nếu file không tồn tại thì sẽ có thông báo file không thể mở được.
	Khi chọn File/Save, một thông báo hiện lên rằng file đã được lưu với tên mặc định "fileOut.txt".
	Khi chọn File/New, một thông báo hiện lên hỏi phần text đang được chỉnh sửa trên edit control
	 đã được lưu chưa, chọn Cancel để hủy lệnh Open để lưu text cũ, chọn OK để edit control được
	 làm sạch để soạn text mới.
	Trong menu Edit là các tác vụ Copy, Cut, Paste, Delete.
-Sự kiện phụ:
	Khi lưu một text vào file, nếu có ký tự unicode thì chương trình chỉ đọc được những ký tự không
	 phải unicode đầu tiên cho tới một ký tự unicode thì dừng lại.
	Khi đọc một file .txt, nếu file được lưu bằng unicode thì chương trình chỉ đọc được ký tự
	 đầu tiên.
	Khi đang chỉnh sửa trên edit control, tắt chương trình thì text sẽ không được lưu, không có
	 thông báo nào trong trường hợp này.
-Link bitbucket: https://bitbucket.org/thaibalong7/windev/src/f17c36b330102159d342cf9c7740dd79405c1898/1512296.Notepad/?at=master













