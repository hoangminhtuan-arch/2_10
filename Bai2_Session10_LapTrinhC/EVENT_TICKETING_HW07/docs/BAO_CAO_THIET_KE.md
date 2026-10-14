# BÁO CÁO PHÂN TÍCH VÀ THIẾT KẾ GIẢI PHÁP
## Bài tập 02: Thiết kế Kiến trúc Module Hàm CRUD Hoàn chỉnh cho TicketBox
**Sự kiện**: Live Concert VIBE 2025

---

### 1. Phân tích I/O (Input / Output) các hàm xử lý

| Tên hàm | Tham số đầu vào (Input) | Giá trị trả về (Output) | Mục đích nghiệp vụ |
| :--- | :--- | :--- | :--- |
| `tinhGiaThanhToan` | - `float giaGoc`: Giá niêm yết vé.<br>- `int isEarlyBird`: Cờ ưu đãi (1: có, 0: không). | `float`: Giá vé thực tế sau chiết khấu. | Tính toán giá tiền thanh toán: giảm 15% nếu mua đợt sớm (Early Bird), ngược lại giữ nguyên giá gốc. |
| `timVeTheoMa` | - `const struct VeSuKien dsVe[]`: Mảng vé (chỉ đọc).<br>- `int soLuongVe`: Số vé hiện có.<br>- `int maVe`: Mã vé cần tìm. | `int`: Vị trí chỉ số (index `0` đến `soLuongVe - 1`) nếu tìm thấy; trả về `-1` nếu không tồn tại. | Hàm tìm kiếm hạt nhân phục vụ tái sử dụng logic cho các chức năng kiểm tra, soát vé và hủy vé. |
| `themVeMoi` | - `struct VeSuKien dsVe[]`: Mảng vé.<br>- `int soLuongVe`: Số vé hiện tại.<br>- `struct VeSuKien veMoi`: Dữ liệu vé cần thêm. | `int`: Số lượng vé mới sau khi thêm (`soLuongVe + 1` nếu thành công; giữ nguyên `soLuongVe` nếu mảng đầy). | Thêm vé vào cuối danh sách và kiểm soát ranh giới tràn mảng (`MAX_VE`). |
| `checkInVe` | - `struct VeSuKien dsVe[]`: Mảng vé.<br>- `int soLuongVe`: Số vé hiện tại.<br>- `int maVe`: Mã vé quét qua cổng QR. | `void`: Cập nhật trực tiếp trường `daCheckIn` của phần tử trong mảng. | Kiểm tra tính hợp lệ của mã QR tại cổng, ngăn chặn gian lận check-in trùng lặp (Duplicate QR Scan). |
| `huyVeTheoMa` | - `struct VeSuKien dsVe[]`: Mảng vé.<br>- `int soLuongVe`: Số vé hiện tại.<br>- `int maVe`: Mã vé yêu cầu hủy. | `int`: Số lượng vé mới sau khi hủy (`soLuongVe - 1` nếu thành công; giữ nguyên nếu mã vé không tồn tại). | Xóa vé bằng thuật toán dồn mảng và cập nhật lại số lượng vé cho `main()`. |
| `hienThiDanhSachVe` | - `const struct VeSuKien dsVe[]`: Mảng vé.<br>- `int soLuongVe`: Số vé hiện có. | `void`: In bảng biểu ra màn hình console. | Hiển thị thông tin vé, giá thực tế và trạng thái qua cổng kiểm soát. |

---

### 2. Thiết kế kiến trúc hàm

#### 2.1. Danh sách Function Prototypes
```c
float tinhGiaThanhToan(float giaGoc, int isEarlyBird);
int timVeTheoMa(const struct VeSuKien dsVe[], int soLuongVe, int maVe);
int themVeMoi(struct VeSuKien dsVe[], int soLuongVe, struct VeSuKien veMoi);
void checkInVe(struct VeSuKien dsVe[], int soLuongVe, int maVe);
int huyVeTheoMa(struct VeSuKien dsVe[], int soLuongVe, int maVe);
void hienThiDanhSachVe(const struct VeSuKien dsVe[], int soLuongVe);
```

#### 2.2. Vai trò của hàm tìm kiếm hạt nhân (`timVeTheoMa`)
- **Nguyên tắc Đơn nhiệm (Single Responsibility Principle - SRP)**: Tách riêng việc định vị vị trí dữ liệu ra khỏi logic nghiệp vụ (soát vé hay xóa vé).
- **Tái sử dụng logic (Code Reuse)**:
  - Cả chức năng **Check-in** và **Hủy vé** đều yêu cầu kiểm tra xem mã vé có tồn tại trong hệ thống hay không trước khi thao tác.
  - Thay vì viết lại vòng lặp duyệt mảng nhiều lần, hai hàm trên chỉ cần gọi `int viTri = timVeTheoMa(dsVe, soLuongVe, maVe);`.
  - Nếu `viTri == -1`, thông báo mã vé không tồn tại và kết thúc ngay.
  - Nếu `viTri >= 0`, hàm gọi chỉ việc truy xuất trực tiếp vào `dsVe[viTri]` để xử lý tiếp, loại bỏ hoàn toàn mã trùng lặp (DRY).

---

### 3. Sơ đồ và các bước xử lý logic chi tiết

#### 3.1. Luồng "Check-in QR" (`checkInVe`)
1. **Bước 1 (Kiểm tra mảng rỗng)**: Nếu `soLuongVe <= 0`, xuất thông báo hệ thống chưa có vé nào và dừng lại.
2. **Bước 2 (Tìm kiếm mã vé)**: Gọi `viTri = timVeTheoMa(dsVe, soLuongVe, maVe)`.
3. **Bước 3 (Xử lý kết quả tìm kiếm)**:
   - Nếu `viTri == -1`: Xuất cảnh báo `[LỖI]: Mã vé không tồn tại trên hệ thống!`.
   - Nếu `viTri >= 0`: Chuyển sang Bước 4.
4. **Bước 4 (Kiểm tra trạng thái quét)**:
   - Nếu `dsVe[viTri].daCheckIn == 1`: Xuất cảnh báo gian lận `[CẢNH BÁO]: Vé đã được check-in trước đó! Từ chối vào cổng!`.
   - Nếu `dsVe[viTri].daCheckIn == 0`: Cập nhật `dsVe[viTri].daCheckIn = 1` và thông báo `[THÀNH CÔNG]: Check-in hợp lệ. Chào mừng quý khách!`.

#### 3.2. Luồng "Hủy vé" (`huyVeTheoMa`)
1. **Bước 1 (Kiểm tra mảng rỗng)**: Nếu `soLuongVe <= 0`, xuất thông báo không có vé để hủy và trả về `0`.
2. **Bước 2 (Tìm kiếm mã vé)**: Gọi `viTri = timVeTheoMa(dsVe, soLuongVe, maVe)`.
3. **Bước 3 (Xử lý kết quả tìm kiếm)**:
   - Nếu `viTri == -1`: Xuất thông báo `[LỖI]: Không tìm thấy mã vé cần hủy!` và trả về `soLuongVe` nguyên vẹn.
   - Nếu `viTri >= 0`: Chuyển sang Bước 4.
4. **Bước 4 (Dồn mảng để xóa phần tử)**:
   - Chạy vòng lặp từ `i = viTri` đến `soLuongVe - 2`, gán `dsVe[i] = dsVe[i + 1]`.
5. **Bước 5 (Đồng bộ số lượng ngoài main)**:
   - Thông báo hủy vé thành công và `return soLuongVe - 1`.
   - Ngoài hàm `main()`, cập nhật biến trạng thái: `soLuongVe = huyVeTheoMa(dsVe, soLuongVe, maCanHuy);`.
