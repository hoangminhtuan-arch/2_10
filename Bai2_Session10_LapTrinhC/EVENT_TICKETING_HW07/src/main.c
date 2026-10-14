#include <stdio.h>
#include <string.h>

#define MAX_VE 50

struct VeSuKien {
    int maVe;
    char tenSuKien[50];
    char loaiVe[20];
    float giaGoc;
    int isEarlyBird;
    int daCheckIn;
};

float tinhGiaThanhToan(float giaGoc, int isEarlyBird);
int timVeTheoMa(const struct VeSuKien dsVe[], int soLuongVe, int maVe);
int themVeMoi(struct VeSuKien dsVe[], int soLuongVe, struct VeSuKien veMoi);
void checkInVe(struct VeSuKien dsVe[], int soLuongVe, int maVe);
int huyVeTheoMa(struct VeSuKien dsVe[], int soLuongVe, int maVe);
void hienThiDanhSachVe(const struct VeSuKien dsVe[], int soLuongVe);

int main() {
    struct VeSuKien dsVe[MAX_VE];
    int soLuongVe = 0;

    printf("=== HE THONG QUAN LY VA SOAT VE CONCERT VIBE 2025 ===\n\n");

    struct VeSuKien v1 = {1001, "Live Concert VIBE 2025", "VIP", 2500000.0f, 1, 0};
    struct VeSuKien v2 = {1002, "Live Concert VIBE 2025", "Zone A", 1500000.0f, 0, 0};
    struct VeSuKien v3 = {1003, "Live Concert VIBE 2025", "GA", 800000.0f, 1, 0};

    soLuongVe = themVeMoi(dsVe, soLuongVe, v1);
    soLuongVe = themVeMoi(dsVe, soLuongVe, v2);
    soLuongVe = themVeMoi(dsVe, soLuongVe, v3);

    printf("\n>>> DANH SACH VE KHOI TAO BAN DAU <<<\n");
    hienThiDanhSachVe(dsVe, soLuongVe);

    printf("\n>>> THAO TAC 1: PHAT HANH THEM VE MOI <<<\n");
    struct VeSuKien v4 = {1004, "Live Concert VIBE 2025", "Zone A", 1500000.0f, 0, 0};
    soLuongVe = themVeMoi(dsVe, soLuongVe, v4);

    printf("\n>>> THAO TAC 2: SOAT VE CHECK-IN HOP LE (MA 1001) <<<\n");
    checkInVe(dsVe, soLuongVe, 1001);

    printf("\n>>> THAO TAC 3: QUET LAI MA VE 1001 (KIEM TRA TRUNG LAP QR) <<<\n");
    checkInVe(dsVe, soLuongVe, 1001);

    printf("\n>>> THAO TAC 4: HUY VE THEO MA (MA 1002) <<<\n");
    soLuongVe = huyVeTheoMa(dsVe, soLuongVe, 1002);

    printf("\n>>> THAO TAC 5: QUET MA VE KHONG TON TAI (MA 9999) <<<\n");
    checkInVe(dsVe, soLuongVe, 9999);

    printf("\n>>> DANH SACH VE HIEN TAI SAU KHI XU LY <<<\n");
    hienThiDanhSachVe(dsVe, soLuongVe);

    return 0;
}

float tinhGiaThanhToan(float giaGoc, int isEarlyBird) {
    if (isEarlyBird == 1) {
        return giaGoc * 0.85f;
    }
    return giaGoc;
}

int timVeTheoMa(const struct VeSuKien dsVe[], int soLuongVe, int maVe) {
    for (int i = 0; i < soLuongVe; i++) {
        if (dsVe[i].maVe == maVe) {
            return i;
        }
    }
    return -1;
}

int themVeMoi(struct VeSuKien dsVe[], int soLuongVe, struct VeSuKien veMoi) {
    if (soLuongVe >= MAX_VE) {
        printf("[LOI]: He thong da dat gioi han luu tru toi da %d ve!\n", MAX_VE);
        return soLuongVe;
    }

    if (timVeTheoMa(dsVe, soLuongVe, veMoi.maVe) != -1) {
        printf("[LOI]: Ma ve %d da ton tai tren he thong!\n", veMoi.maVe);
        return soLuongVe;
    }

    dsVe[soLuongVe] = veMoi;
    soLuongVe++;
    printf("[THANH CONG]: Da phat hanh ve %d (%s) - %s\n", veMoi.maVe, veMoi.loaiVe, veMoi.tenSuKien);
    return soLuongVe;
}

void checkInVe(struct VeSuKien dsVe[], int soLuongVe, int maVe) {
    if (soLuongVe <= 0) {
        printf("[LOI]: He thong hien chua co ve nao de check-in!\n");
        return;
    }

    int viTri = timVeTheoMa(dsVe, soLuongVe, maVe);
    if (viTri == -1) {
        printf("[LOI]: Khong tim thay ma ve %d trong he thong!\n", maVe);
        return;
    }

    if (dsVe[viTri].daCheckIn == 1) {
        printf("[CANH BAO GIAN LAN]: Ma ve %d da duoc check-in truoc do! Tu choi vao cong!\n", maVe);
        return;
    }

    dsVe[viTri].daCheckIn = 1;
    printf("[THANH CONG]: Check-in hop le cho ve %d (%s). Chuc quy khach xem concert vui ve!\n",
           dsVe[viTri].maVe, dsVe[viTri].loaiVe);
}

int huyVeTheoMa(struct VeSuKien dsVe[], int soLuongVe, int maVe) {
    if (soLuongVe <= 0) {
        printf("[LOI]: He thong hien chua co ve nao de huy!\n");
        return soLuongVe;
    }

    int viTri = timVeTheoMa(dsVe, soLuongVe, maVe);
    if (viTri == -1) {
        printf("[LOI]: Khong tim thay ma ve %d can huy!\n", maVe);
        return soLuongVe;
    }

    for (int i = viTri; i < soLuongVe - 1; i++) {
        dsVe[i] = dsVe[i + 1];
    }
    soLuongVe--;
    printf("[THANH CONG]: Da huy va xoa ve ma %d khoi he thong.\n", maVe);
    return soLuongVe;
}

void hienThiDanhSachVe(const struct VeSuKien dsVe[], int soLuongVe) {
    if (soLuongVe <= 0) {
        printf("[THONG BAO]: Danh sach ve dang trong!\n");
        return;
    }

    printf("-----------------------------------------------------------------------------------------\n");
    printf("%-5s %-10s %-12s %-15s %-15s %-15s\n",
           "STT", "MA VE", "KHU VUC", "GIA GOC", "THANH TOAN", "CHECK-IN");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuongVe; i++) {
        float giaThucTe = tinhGiaThanhToan(dsVe[i].giaGoc, dsVe[i].isEarlyBird);
        char trangThaiCheckIn[20];
        if (dsVe[i].daCheckIn == 1) {
            strcpy(trangThaiCheckIn, "Da Check-in");
        } else {
            strcpy(trangThaiCheckIn, "Chua Check-in");
        }

        printf("%-5d %-10d %-12s %-15.0f %-15.0f %-15s\n",
               i + 1,
               dsVe[i].maVe,
               dsVe[i].loaiVe,
               dsVe[i].giaGoc,
               giaThucTe,
               trangThaiCheckIn);
    }
    printf("-----------------------------------------------------------------------------------------\n");
    printf("Tong so ve hien co: %d\n", soLuongVe);
}
