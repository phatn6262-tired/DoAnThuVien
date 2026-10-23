#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <conio.h>

using namespace std;

// ===================== ma phim nong =====================
#define KEY_UP       72
#define KEY_DOWN     80
#define KEY_LEFT     75
#define KEY_RIGHT    77
#define KEY_ENTER    13
#define KEY_ESC      27
#define KEY_INSERT   82
#define KEY_DELETE   83
#define KEY_F1       59
#define KEY_F2       60
#define KEY_F3       61
#define KEY_F5       63

// ===================== hang so du lieu =====================
const int MAXSACH        = 10000;
const int MAX_MUON_SACH  = 3;
const int HAN_MUON_NGAY  = 7;
const int LINES_PER_PAGE = 10;

const char FILE_DAUSACH[] = "DAUSACH.TXT";
const char FILE_DOCGIA[]  = "THEDOCGIA.TXT";

// ===================== do hoa co ban =====================
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int textColor, int bgColor = 0) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgColor << 4) | textColor);
}

void showCursor(bool visible) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20;
    info.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void clearArea(int x, int y, int width, int height) {
    setColor(15, 0);
    for (int i = 0; i < height; ++i) {
        gotoxy(x, y + i);
        for (int j = 0; j < width; ++j) cout << " ";
    }
}

// ===================== cau truc ngay thang =====================
struct NgayThang {
    int Ngay;
    int Thang;
    int Nam;

    NgayThang() : Ngay(0), Thang(0), Nam(0) {}
    NgayThang(int d, int m, int y) : Ngay(d), Thang(m), Nam(y) {}
};

int NamNhuan(int Nam) {
    return (Nam % 400 == 0 || (Nam % 4 == 0 && Nam % 100 != 0)) ? 1 : 0;
}

int SoNgayTrongThang(int Thang, int Nam) {
    if (Thang == 4 || Thang == 6 || Thang == 9 || Thang == 11) return 30;
    if (Thang == 2) return NamNhuan(Nam) ? 29 : 28;
    return 31;
}

long long NgayToSoNgay(NgayThang d) {
    long long tong = 0;
    for (int y = 1; y < d.Nam; y++) tong += NamNhuan(y) ? 366 : 365;
    for (int m = 1; m < d.Thang; m++) tong += SoNgayTrongThang(m, d.Nam);
    tong += d.Ngay;
    return tong;
}

int SoNgay(NgayThang NgayMuon, NgayThang NgayHienTai) {
    return (int)(NgayToSoNgay(NgayHienTai) - NgayToSoNgay(NgayMuon));
}

NgayThang LayNgayHienTai() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    return NgayThang(ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
}

// ===================== danh muc muon sach =====================
struct ThongTinSach {
    char MaSach[30];
    int TrangThai; // 0: cho muon duoc, 1: da muon, 2: thanh ly
    char ViTri[30];

    ThongTinSach() {
        MaSach[0] = '\0';
        TrangThai = 0;
        ViTri[0] = '\0';
    }
};

struct DanhMucSach {
    ThongTinSach data;
    DanhMucSach* next;
};
typedef DanhMucSach* PTR_DanhMucSach;

// ===================== dau sach =====================
struct DauSach {
    char ISBN[20];
    char TenSach[60];
    int SoTrang;
    char TacGia[40];
    int NamXuatBan;
    char TheLoai[30];
    int SoLuotMuon;
    PTR_DanhMucSach dms;

    DauSach() {
        ISBN[0] = '\0'; TenSach[0] = '\0'; SoTrang = 0;
        TacGia[0] = '\0'; NamXuatBan = 0; TheLoai[0] = '\0';
        SoLuotMuon = 0; dms = nullptr;
    }
};

struct DS_DauSach {
    int n;
    DauSach* ds[MAXSACH];
    DS_DauSach() : n(0) {
        for (int i = 0; i < MAXSACH; ++i) ds[i] = nullptr;
    }
};

// ===================== muon tra =====================
struct MuonTra {
    char MaSach[30];
    NgayThang NgayMuon;
    NgayThang NgayTra;
    int TrangThai; // 0: dang muon, 1: da tra, 2: lam mat

    MuonTra() {
        MaSach[0] = '\0';
        TrangThai = 0;
    }
};

struct DS_MuonTra {
    MuonTra data;
    DS_MuonTra* next;
};
typedef DS_MuonTra* PTR_MuonTra;

// ===================== the doc gia =====================
struct TheDocGia {
    int MaThe;
    char Ho[40];
    char Ten[20];
    char Phai[5];
    int TrangThai; // 0: bi khoa, 1: hoat dong
    PTR_MuonTra mt;

    TheDocGia() {
        MaThe = 0; Ho[0] = '\0'; Ten[0] = '\0';
        Phai[0] = '\0'; TrangThai = 1; mt = nullptr;
    }
};

struct DS_TheDocGia {
    TheDocGia data;
    DS_TheDocGia *left, *right;
};
typedef DS_TheDocGia* PTR_TheDocGia;

// ===================== thuat toan co so =====================

// CAU A
PTR_TheDocGia TimMaThe(PTR_TheDocGia root, int MaThe) {
    if (root == nullptr || root->data.MaThe == MaThe) return root;
    if (MaThe < root->data.MaThe) return TimMaThe(root->left, MaThe);
    return TimMaThe(root->right, MaThe);
}

int SinhMaThe(PTR_TheDocGia root) {
    int ma;
    do {
        ma = rand() % 90000 + 10000;
    } while (TimMaThe(root, ma) != nullptr);
    return ma;
}

bool InsertDocGia(PTR_TheDocGia& p, const TheDocGia& TDG) {
    if (p == nullptr) {
        p = new DS_TheDocGia;
        p->data = TDG;
        p->left = p->right = nullptr;
        return true;
    }
    if (TDG.MaThe < p->data.MaThe) return InsertDocGia(p->left, TDG);
    if (TDG.MaThe > p->data.MaThe) return InsertDocGia(p->right, TDG);
    return false;
}

void TimLop(PTR_TheDocGia& p, PTR_TheDocGia& q) {
    if (q->left != nullptr) {
        TimLop(p, q->left);
    } else {
        p->data = q->data;
        PTR_TheDocGia temp = q;
        q = q->right;
        delete temp;
    }
}

bool DelDocGia(PTR_TheDocGia& p, int MaThe) {
    if (p == nullptr) return false;
    if (MaThe < p->data.MaThe) return DelDocGia(p->left, MaThe);
    if (MaThe > p->data.MaThe) return DelDocGia(p->right, MaThe);

    // Kiem tra xem doc gia co dang muon sach chua tra khong
    for (PTR_MuonTra cur = p->data.mt; cur != nullptr; cur = cur->next) {
        if (cur->data.TrangThai == 0) return false;
    }

    PTR_TheDocGia temp = p;
    if (p->left == nullptr) p = p->right;
    else if (p->right == nullptr) p = p->left;
    else TimLop(temp, p->right);

    if (temp != p) delete temp;
    return true;
}

void ChuyenCayVaoMang(PTR_TheDocGia root, TheDocGia* arr[], int& count) {
    if (root == nullptr) return;
    ChuyenCayVaoMang(root->left, arr, count);
    arr[count++] = &(root->data);
    ChuyenCayVaoMang(root->right, arr, count);
}

int DemSoSachCuaDauSach(PTR_DanhMucSach head) {
    int dem = 0;
    for (PTR_DanhMucSach p = head; p != nullptr; p = p->next) dem++;
    return dem;
}

bool ChenDauSachCoThuTu(DS_DauSach& DS, DauSach* dsMoi) {
    if (DS.n >= MAXSACH) return false;
    int pos = DS.n;
    while (pos > 0 && strcmp(DS.ds[pos - 1]->TenSach, dsMoi->TenSach) > 0) {
        DS.ds[pos] = DS.ds[pos - 1];
        pos--;
    }
    DS.ds[pos] = dsMoi;
    DS.n++;
    return true;
}

void ThemSachChoDauSach(DauSach* ds, int soLuongThem, const char* viTri) {
    int stt = DemSoSachCuaDauSach(ds->dms) + 1;
    for (int i = 0; i < soLuongThem; ++i) {
        PTR_DanhMucSach p = new DanhMucSach;
        sprintf(p->data.MaSach, "%s_%d", ds->ISBN, stt++);
        p->data.TrangThai = 0;
        strcpy(p->data.ViTri, viTri);
        p->next = nullptr;

        if (ds->dms == nullptr) {
            ds->dms = p;
        } else {
            PTR_DanhMucSach cur = ds->dms;
            while (cur->next != nullptr) cur = cur->next;
            cur->next = p;
        }
    }
}

void LayISBNTuMaSach(const char* maSach, char* isbnOut) {
    int i = 0;
    while (maSach[i] != '_' && maSach[i] != '\0') {
        isbnOut[i] = maSach[i];
        i++;
    }
    isbnOut[i] = '\0';
}

int TimDauSachTheoISBN(const DS_DauSach& DS, const char* isbn) {
    for (int i = 0; i < DS.n; ++i) {
        if (strcmp(DS.ds[i]->ISBN, isbn) == 0) return i;
    }
    return -1;
}

ThongTinSach* TimSachTheoMa(const DS_DauSach& DS, const char* maSach, DauSach*& dauSachChua) {
    char isbn[20];
    LayISBNTuMaSach(maSach, isbn);
    int idx = TimDauSachTheoISBN(DS, isbn);
    if (idx == -1) return nullptr;
    dauSachChua = DS.ds[idx];
    for (PTR_DanhMucSach p = dauSachChua->dms; p != nullptr; p = p->next) {
        if (strcmp(p->data.MaSach, maSach) == 0) return &(p->data);
    }
    return nullptr;
}

int CoQuaHan(PTR_TheDocGia p) {
    NgayThang ht = LayNgayHienTai();
    for (PTR_MuonTra q = p->data.mt; q != nullptr; q = q->next) {
        if (q->data.TrangThai == 0) {
            if (SoNgay(q->data.NgayMuon, ht) > HAN_MUON_NGAY) return 1;
        }
    }
    return 0;
}

// ===================== luu file =====================
void SaveDauSach(const DS_DauSach& DS) {
    ofstream out(FILE_DAUSACH);
    if (!out.is_open()) return;
    out << DS.n << "\n";
    for (int i = 0; i < DS.n; ++i) {
        int cntDMS = DemSoSachCuaDauSach(DS.ds[i]->dms);
        out << DS.ds[i]->ISBN << "|" << DS.ds[i]->TenSach << "|" 
            << DS.ds[i]->SoTrang << "|" << DS.ds[i]->TacGia << "|" 
            << DS.ds[i]->NamXuatBan << "|" << DS.ds[i]->TheLoai << "|" 
            << DS.ds[i]->SoLuotMuon << "|" << cntDMS << "\n";
        for (PTR_DanhMucSach p = DS.ds[i]->dms; p != nullptr; p = p->next) {
            out << p->data.MaSach << "|" << p->data.TrangThai << "|" << p->data.ViTri << "\n";
        }
    }
    out.close();
}

void LoadDauSach(DS_DauSach& DS) {
    ifstream in(FILE_DAUSACH);
    if (!in.is_open()) return;
    int soDS = 0;
    if (!(in >> soDS)) return;
    in.ignore();
    for (int i = 0; i < soDS; ++i) {
        DauSach* p = new DauSach();
        string temp;
        in.getline(p->ISBN, 20, '|');
        in.getline(p->TenSach, 60, '|');
        getline(in, temp, '|'); p->SoTrang = atoi(temp.c_str());
        in.getline(p->TacGia, 40, '|');
        getline(in, temp, '|'); p->NamXuatBan = atoi(temp.c_str());
        in.getline(p->TheLoai, 30, '|');
        getline(in, temp, '|'); p->SoLuotMuon = atoi(temp.c_str());
        int cntDMS = 0;
        in >> cntDMS;
        in.ignore();
        p->dms = nullptr;
        for (int j = 0; j < cntDMS; ++j) {
            PTR_DanhMucSach nodeS = new DanhMucSach;
            in.getline(nodeS->data.MaSach, 30, '|');
            getline(in, temp, '|'); nodeS->data.TrangThai = atoi(temp.c_str());
            in.getline(nodeS->data.ViTri, 30);
            nodeS->next = nullptr;
            if (p->dms == nullptr) p->dms = nodeS;
            else {
                PTR_DanhMucSach tail = p->dms;
                while (tail->next != nullptr) tail = tail->next;
                tail->next = nodeS;
            }
        }
        ChenDauSachCoThuTu(DS, p);
    }
    in.close();
}

void GhiDocGiaNLR(PTR_TheDocGia root, ofstream& out) {
    if (!root) return;
    int cntMT = 0;
    for (PTR_MuonTra p = root->data.mt; p != nullptr; p = p->next) cntMT++;
    out << root->data.MaThe << "|" << root->data.Ho << "|" << root->data.Ten << "|" 
        << root->data.Phai << "|" << root->data.TrangThai << "|" << cntMT << "\n";
    for (PTR_MuonTra p = root->data.mt; p != nullptr; p = p->next) {
        out << p->data.MaSach << "|" 
            << p->data.NgayMuon.Ngay << "|" << p->data.NgayMuon.Thang << "|" << p->data.NgayMuon.Nam << "|" 
            << p->data.NgayTra.Ngay << "|" << p->data.NgayTra.Thang << "|" << p->data.NgayTra.Nam << "|" 
            << p->data.TrangThai << "\n";
    }
    GhiDocGiaNLR(root->left, out);
    GhiDocGiaNLR(root->right, out);
}

int DemSoDocGia(PTR_TheDocGia root) {
    if (!root) return 0;
    return 1 + DemSoDocGia(root->left) + DemSoDocGia(root->right);
}

void SaveTheDocGia(PTR_TheDocGia root) {
    ofstream out(FILE_DOCGIA);
    if (!out.is_open()) return;
    out << DemSoDocGia(root) << "\n";
    GhiDocGiaNLR(root, out);
    out.close();
}

void LoadTheDocGia(PTR_TheDocGia& root) {
    ifstream in(FILE_DOCGIA);
    if (!in.is_open()) return;
    int soDG = 0;
    if (!(in >> soDG)) return;
    in.ignore();
    for (int i = 0; i < soDG; ++i) {
        TheDocGia dg;
        string temp;
        getline(in, temp, '|'); dg.MaThe = atoi(temp.c_str());
        in.getline(dg.Ho, 40, '|');
        in.getline(dg.Ten, 20, '|');
        in.getline(dg.Phai, 5, '|');
        getline(in, temp, '|'); dg.TrangThai = atoi(temp.c_str());
        int cntMT = 0;
        in >> cntMT;
        in.ignore();
        dg.mt = nullptr;
        for (int j = 0; j < cntMT; ++j) {
            PTR_MuonTra nodeMT = new DS_MuonTra;
            in.getline(nodeMT->data.MaSach, 30, '|');
            getline(in, temp, '|'); nodeMT->data.NgayMuon.Ngay = atoi(temp.c_str());
            getline(in, temp, '|'); nodeMT->data.NgayMuon.Thang = atoi(temp.c_str());
            getline(in, temp, '|'); nodeMT->data.NgayMuon.Nam = atoi(temp.c_str());
            getline(in, temp, '|'); nodeMT->data.NgayTra.Ngay = atoi(temp.c_str());
            getline(in, temp, '|'); nodeMT->data.NgayTra.Thang = atoi(temp.c_str());
            getline(in, temp, '|'); nodeMT->data.NgayTra.Nam = atoi(temp.c_str());
            in >> nodeMT->data.TrangThai;
            in.ignore();
            nodeMT->next = nullptr;
            if (dg.mt == nullptr) dg.mt = nodeMT;
            else {
                PTR_MuonTra tail = dg.mt;
                while (tail->next != nullptr) tail = tail->next;
                tail->next = nodeMT;	
            }
        }
        InsertDocGia(root, dg);
    }
    in.close();
}

// ===================== khung giao dien =====================
void drawBanner(int x, int y, const char* title) {
    setColor(0, 14); // Ch? den, n?n v�ng
    gotoxy(x, y);
    cout << "  " << title << "  ";
    setColor(15, 0);
}

void drawNavigationGuide(int x, int y) {
    setColor(14, 0);
    gotoxy(x + 4, y);     cout << "^";
    gotoxy(x, y + 2);     cout << "<       >";
    gotoxy(x + 4, y + 4); cout << "v";

    setColor(15, 0);
    gotoxy(x - 2, y + 6); cout << "UP    : LEN";
    gotoxy(x - 2, y + 8); cout << "DOWN  : XUONG";
    gotoxy(x - 2, y + 10);cout << "LEFT  : TRAI";
    gotoxy(x - 2, y + 12);cout << "RIGHT : PHAI";
}

void drawBottomBar(int y, const char* f5Text = "F5: Xem") {
    gotoxy(3, y);
    setColor(12, 15); cout << " ESC "; setColor(15, 0); cout << ": Exit   ";
    setColor(12, 15); cout << " ENTER "; setColor(15, 0); cout << ": OK   ";
    setColor(12, 15); cout << " F2 "; setColor(15, 0); cout << ": Save   ";
    setColor(12, 15); cout << " INS "; setColor(15, 0); cout << ": Them   ";
    setColor(12, 15); cout << " DEL "; setColor(15, 0); cout << ": Xoa   ";
    setColor(12, 15); cout << " F5 "; setColor(15, 0); cout << ": " << f5Text << "   ";
}

void drawTableFrame(int x, int y, int colWidths[], int numCols, int numRows) {
    setColor(14, 0);
    gotoxy(x, y);
    cout << "+";
    for (int c = 0; c < numCols; ++c) {
        for (int i = 0; i < colWidths[c]; ++i) cout << "-";
        cout << "+";
    }

    gotoxy(x, y + 2);
    cout << "+";
    for (int c = 0; c < numCols; ++c) {
        for (int i = 0; i < colWidths[c]; ++i) cout << "-";
        cout << "+";
    }

    for (int r = 0; r < numRows + 3; ++r) {
        if (r == 0 || r == 2) continue;
        gotoxy(x, y + r);
        cout << "|";
        for (int c = 0; c < numCols; ++c) {
            for (int i = 0; i < colWidths[c]; ++i) cout << " ";
            cout << "|";
        }
    }

    gotoxy(x, y + numRows + 3);
    cout << "+";
    for (int c = 0; c < numCols; ++c) {
        for (int i = 0; i < colWidths[c]; ++i) cout << "-";
        cout << "+";
    }
    setColor(15, 0);
}

// ===================== mh1 quan li dau sach =====================
void viewDauSach(DS_DauSach& DS) {
    showCursor(false);
    system("cls");

    drawBanner(25, 2, "DANH SACH DAU SACH THU VIEN (MANG CON TRO)");

    int colW[] = { 6, 12, 35, 10, 10, 18 };
    int numCols = 6;
    drawTableFrame(3, 4, colW, numCols, LINES_PER_PAGE);

    gotoxy(5, 5);  cout << "STT";
    gotoxy(12, 5); cout << "ISBN";
    gotoxy(25, 5); cout << "TEN SACH";
    gotoxy(61, 5); cout << "SO TRANG";
    gotoxy(72, 5); cout << "NAM XB";
    gotoxy(83, 5); cout << "THE LOAI";

    drawNavigationGuide(104, 8);
    drawBottomBar(19, "F5: The Loai");

    int totalPages = (DS.n == 0) ? 1 : ((DS.n - 1) / LINES_PER_PAGE + 1);
    int curPage = 1;
    int curRow = 0;

    bool exitView = false;
    while (!exitView) {
        int startIdx = (curPage - 1) * LINES_PER_PAGE;
        int itemsOnPage = min(LINES_PER_PAGE, DS.n - startIdx);

        for (int i = 0; i < LINES_PER_PAGE; ++i) {
            int idx = startIdx + i;
            gotoxy(4, 7 + i);
            if (i < itemsOnPage) {
                if (i == curRow) setColor(0, 11);
                else setColor(15, 0);

                char buf[120];
                sprintf(buf, " %-4d| %-10s| %-33.33s| %-8d| %-8d| %-16.16s", 
                        idx + 1, DS.ds[idx]->ISBN, DS.ds[idx]->TenSach,
                        DS.ds[idx]->SoTrang, DS.ds[idx]->NamXuatBan, DS.ds[idx]->TheLoai);
                cout << buf;
            } else {
                setColor(15, 0);
                cout << "     |           |                                  |         |         |                 ";
            }
        }

        gotoxy(102, 21);
        setColor(10, 0);
        cout << "Trang : " << curPage << "/" << totalPages << " ";
        setColor(15, 0);

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case KEY_UP:
                    if (curRow > 0) curRow--;
                    break;
                case KEY_DOWN:
                    if (curRow < itemsOnPage - 1) curRow++;
                    break;
                case KEY_LEFT:
                    if (curPage > 1) { curPage--; curRow = 0; }
                    break;
                case KEY_RIGHT:
                    if (curPage < totalPages) { curPage++; curRow = 0; }
                    break;
                case KEY_INSERT: { // [C�U C]: Th�m d?u s�ch m?i
                    showCursor(true);
                    clearArea(3, 21, 95, 7);
                    setColor(14, 0);
                    gotoxy(3, 21); cout << "--- NHAP DAU SACH MOI ---";
                    DauSach* p = new DauSach();
                    gotoxy(3, 22); cout << "ISBN: "; cin.getline(p->ISBN, 20);
                    gotoxy(3, 23); cout << "Ten sach: "; cin.getline(p->TenSach, 60);
                    gotoxy(3, 24); cout << "So trang: "; cin >> p->SoTrang; cin.ignore();
                    gotoxy(3, 25); cout << "Tac gia: "; cin.getline(p->TacGia, 40);
                    gotoxy(45, 24); cout << "Nam XB: "; cin >> p->NamXuatBan; cin.ignore();
                    gotoxy(45, 25); cout << "The loai: "; cin.getline(p->TheLoai, 30);
                    
                    int soLuong; char viTri[30];
                    gotoxy(3, 26); cout << "So sach: "; cin >> soLuong; cin.ignore();
                    gotoxy(45, 26); cout << "Vi tri ke: "; cin.getline(viTri, 30);

                    ThemSachChoDauSach(p, soLuong, viTri);
                    ChenDauSachCoThuTu(DS, p);

                    totalPages = (DS.n == 0) ? 1 : ((DS.n - 1) / LINES_PER_PAGE + 1);
                    showCursor(false);
                    clearArea(3, 21, 95, 7);
                    break;
                }
                case KEY_F2:
                    SaveDauSach(DS);
                    gotoxy(3, 21);
                    setColor(10, 0); cout << "Da luu vao file DAUSACH.TXT thanh cong!";
                    Sleep(1000);
                    clearArea(3, 21, 50, 1);
                    break;
                case KEY_F5: { // [CaU D]: In dau sach theo tung the loai
                    system("cls");
                    drawBanner(25, 2, "IN DAU SACH THEO TUNG THE LOAI (TEN TANG DAN)");
                    int y = 5;
                    char tlDaIn[100][30]; int slTL = 0;
                    for (int i = 0; i < DS.n; ++i) {
                        bool daCo = false;
                        for (int j = 0; j < slTL; ++j) {
                            if (stricmp(DS.ds[i]->TheLoai, tlDaIn[j]) == 0) { daCo = true; break; }
                        }
                        if (!daCo) strcpy(tlDaIn[slTL++], DS.ds[i]->TheLoai);
                    }
                    for (int t = 0; t < slTL; ++t) {
                        setColor(14, 0);
                        gotoxy(5, y++); cout << ">> THE LOAI: " << tlDaIn[t];
                        setColor(15, 0);
                        for (int i = 0; i < DS.n; ++i) {
                            if (stricmp(DS.ds[i]->TheLoai, tlDaIn[t]) == 0) {
                                gotoxy(7, y++);
                                cout << "- [" << DS.ds[i]->ISBN << "] " << DS.ds[i]->TenSach 
                                     << " (" << DS.ds[i]->TacGia << ", " << DS.ds[i]->NamXuatBan << ")";
                            }
                        }
                        y++;
                    }
                    gotoxy(5, y); setColor(10, 0); cout << "Nhan phim bat ky de quay lai...";
                    _getch();
                    return viewDauSach(DS);
                }
            }
        } else if (ch == KEY_ESC) {
            exitView = true;
        } else if (ch == KEY_ENTER) { // [C�U E]: Xem chi ti?t danh m?c s�ch con (DMS)
            if (itemsOnPage > 0) {
                int idx = startIdx + curRow;
                system("cls");
                drawBanner(15, 2, "DANH MUC SACH CON THUOC DAU SACH");
                gotoxy(5, 4); setColor(11, 0);
                cout << "Ten sach: " << DS.ds[idx]->TenSach << " | ISBN: " << DS.ds[idx]->ISBN;

                int colDMS[] = { 6, 20, 25, 25 };
                drawTableFrame(5, 6, colDMS, 4, 10);
                gotoxy(7, 7); cout << "STT";
                gotoxy(15, 7); cout << "MA SACH";
                gotoxy(36, 7); cout << "TRANG THAI";
                gotoxy(62, 7); cout << "VI TRI";

                int line = 0;
                for (PTR_DanhMucSach p = DS.ds[idx]->dms; p != nullptr; p = p->next) {
                    gotoxy(7, 9 + line);
                    const char* tt = (p->data.TrangThai == 0) ? "Cho muon duoc" : 
                                     ((p->data.TrangThai == 1) ? "Da co nguoi muon" : "Da thanh ly");
                    char buf[100];
                    sprintf(buf, "%-4d| %-18s| %-23s| %-23s", line + 1, p->data.MaSach, tt, p->data.ViTri);
                    cout << buf;
                    line++;
                }
                drawBottomBar(22);
                _getch();
                return viewDauSach(DS);
            }
        }
    }
}

// ===================== MH 2: quan li the doc gia =====================
void viewTheDocGia(PTR_TheDocGia& root) {
    showCursor(false);
    system("cls");

    drawBanner(25, 2, "DANH SACH THE DOC GIA (CAY NHI PHAN TIM KIEM BST)");

    int colW[] = { 6, 15, 28, 15, 8, 18 };
    int numCols = 6;
    drawTableFrame(3, 4, colW, numCols, LINES_PER_PAGE);

    gotoxy(5, 5);  cout << "STT";
    gotoxy(12, 5); cout << "MA THE";
    gotoxy(28, 5); cout << "HO";
    gotoxy(56, 5); cout << "TEN";
    gotoxy(71, 5); cout << "PHAI";
    gotoxy(80, 5); cout << "TRANG THAI";

    drawNavigationGuide(104, 8);
    drawBottomBar(19, "F5: Xep Ten");

    TheDocGia* arr[MAXSACH];
    int totalDG = 0;
    ChuyenCayVaoMang(root, arr, totalDG);

    int totalPages = (totalDG == 0) ? 1 : ((totalDG - 1) / LINES_PER_PAGE + 1);
    int curPage = 1;
    int curRow = 0;

    bool exitView = false;
    while (!exitView) {
        int startIdx = (curPage - 1) * LINES_PER_PAGE;
        int itemsOnPage = min(LINES_PER_PAGE, totalDG - startIdx);

        for (int i = 0; i < LINES_PER_PAGE; ++i) {
            int idx = startIdx + i;
            gotoxy(4, 7 + i);
            if (i < itemsOnPage) {
                if (i == curRow) setColor(0, 11);
                else setColor(15, 0);

                const char* tt = (arr[idx]->TrangThai == 1) ? "Hoat dong" : "Bi khoa";
                char buf[120];
                sprintf(buf, " %-4d| %-13d| %-26.26s| %-13.13s| %-6s| %-16s", 
                        idx + 1, arr[idx]->MaThe, arr[idx]->Ho, arr[idx]->Ten, arr[idx]->Phai, tt);
                cout << buf;
            } else {
                setColor(15, 0);
                cout << "     |              |                           |              |       |                 ";
            }
        }

        gotoxy(102, 21);
        setColor(10, 0);
        cout << "Trang : " << curPage << "/" << totalPages << " ";
        setColor(15, 0);

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case KEY_UP:
                    if (curRow > 0) curRow--;
                    break;
                case KEY_DOWN:
                    if (curRow < itemsOnPage - 1) curRow++;
                    break;
                case KEY_LEFT:
                    if (curPage > 1) { curPage--; curRow = 0; }
                    break;
                case KEY_RIGHT:
                    if (curPage < totalPages) { curPage++; curRow = 0; }
                    break;
                case KEY_INSERT: { 
                    showCursor(true);
                    clearArea(3, 21, 95, 6);
                    setColor(14, 0);
                    gotoxy(3, 21); cout << "--- THEM THE DOC GIA MOI ---";
                    TheDocGia dg;
                    dg.MaThe = SinhMaThe(root);
                    gotoxy(3, 22); cout << "Ma the (tu dong): " << dg.MaThe;
                    gotoxy(3, 23); cout << "Ho: "; cin.getline(dg.Ho, 40);
                    gotoxy(3, 24); cout << "Ten: "; cin.getline(dg.Ten, 20);
                    do {
                        gotoxy(3, 25); cout << "Phai (Nam/Nu): "; cin.getline(dg.Phai, 5);
                    } while (stricmp(dg.Phai, "Nam") != 0 && stricmp(dg.Phai, "Nu") != 0);
                    dg.TrangThai = 1;
                    dg.mt = nullptr;
                    InsertDocGia(root, dg);

                    totalDG = 0;
                    ChuyenCayVaoMang(root, arr, totalDG);
                    totalPages = (totalDG == 0) ? 1 : ((totalDG - 1) / LINES_PER_PAGE + 1);
                    showCursor(false);
                    clearArea(3, 21, 95, 6);
                    break;
                }
                case KEY_DELETE: { 
                    if (itemsOnPage > 0) {
                        int idx = startIdx + curRow;
                        int maXoa = arr[idx]->MaThe;
                        clearArea(3, 21, 95, 3);
                        gotoxy(3, 21); setColor(12, 0);
                        cout << "Ban co chac muon xoa the doc gia " << maXoa << "? (Y/N): ";
                        setColor(15, 0);
                        int confirm = _getch();
                        if (confirm == 'y' || confirm == 'Y') {
                            if (DelDocGia(root, maXoa)) {
                                gotoxy(3, 22); setColor(10, 0);
                                cout << "=> Da xoa the doc gia thanh cong!";
                            } else {
                                gotoxy(3, 22); setColor(12, 0);
                                cout << "(!) Doc gia dang muon sach hoac khong xoa duoc!";
                            }
                            Sleep(1200);
                            totalDG = 0;
                            ChuyenCayVaoMang(root, arr, totalDG);
                            totalPages = (totalDG == 0) ? 1 : ((totalDG - 1) / LINES_PER_PAGE + 1);
                            if (curRow >= totalDG - startIdx && curRow > 0) curRow--;
                        }
                        clearArea(3, 21, 95, 3);
                    }
                    break;
                }
                case KEY_F3: {
                    if (itemsOnPage > 0) {
                        int idx = startIdx + curRow;
                        showCursor(true);
                        clearArea(3, 21, 95, 6);
                        setColor(14, 0);
                        gotoxy(3, 21); cout << "--- HIEU CHINH DOC GIA MA THE: " << arr[idx]->MaThe << " ---";
                        gotoxy(3, 22); cout << "Ho (" << arr[idx]->Ho << "): "; cin.getline(arr[idx]->Ho, 40);
                        gotoxy(3, 23); cout << "Ten (" << arr[idx]->Ten << "): "; cin.getline(arr[idx]->Ten, 20);
                        do {
                            gotoxy(3, 24); cout << "Phai (" << arr[idx]->Phai << "): "; cin.getline(arr[idx]->Phai, 5);
                        } while (stricmp(arr[idx]->Phai, "Nam") != 0 && stricmp(arr[idx]->Phai, "Nu") != 0);
                        gotoxy(3, 25); cout << "Trang thai the (0: Khoa, 1: Hoat dong): "; cin >> arr[idx]->TrangThai; cin.ignore();
                        
                        showCursor(false);
                        clearArea(3, 21, 95, 6);
                    }
                    break;
                }
                case KEY_F2:
                    SaveTheDocGia(root);
                    gotoxy(3, 21);
                    setColor(10, 0); cout << "Da luu vao file THEDOCGIA.TXT thanh cong!";
                    Sleep(1000);
                    clearArea(3, 21, 50, 1);
                    break;
                case KEY_F5: { // [C�U B]: S?p x?p danh s�ch theo T�n + H? tang d?n
                    for (int i = 0; i < totalDG - 1; ++i) {
                        int minIdx = i;
                        for (int j = i + 1; j < totalDG; ++j) {
                            int cmpTen = stricmp(arr[j]->Ten, arr[minIdx]->Ten);
                            if (cmpTen < 0 || (cmpTen == 0 && stricmp(arr[j]->Ho, arr[minIdx]->Ho) < 0)) minIdx = j;
                        }
                        if (minIdx != i) swap(arr[i], arr[minIdx]);
                    }
                    curPage = 1; curRow = 0;
                    break;
                }
            }
        } else if (ch == KEY_ESC) {
            exitView = true;
        }
    }
}

// ===================== mh 3: muon tra sach =====================
void viewMuonTra(PTR_TheDocGia root, DS_DauSach& DS) {
    system("cls");
    drawBanner(25, 2, "NGHIEP VU MUON - TRA SACH");

    showCursor(true);
    gotoxy(5, 5); setColor(14, 0); cout << "NHAP MA THE DOC GIA: ";
    setColor(15, 0);
    int maThe;
    if (!(cin >> maThe)) {
        cin.clear(); cin.ignore(1000, '\n'); return;
    }
    cin.ignore();

    PTR_TheDocGia pDG = TimMaThe(root, maThe);
    if (!pDG) {
        gotoxy(5, 7); setColor(12, 0); cout << "(!) KHONG TIM THAY MA THE!";
        Sleep(1200); return;
    }

    gotoxy(5, 7); setColor(11, 0);
    cout << "Doc gia: " << pDG->data.Ho << " " << pDG->data.Ten 
         << " | Phai: " << pDG->data.Phai 
         << " | Trang thai: " << (pDG->data.TrangThai == 1 ? "Hoat dong" : "BI KHOA");

    // [C�U H]: B?ng danh s�ch c�c s�ch dang mu?n
    int colMT[] = { 6, 18, 35, 15, 15 };
    drawTableFrame(5, 9, colMT, 5, 4);
    gotoxy(7, 10); cout << "STT";
    gotoxy(15, 10); cout << "MA SACH";
    gotoxy(35, 10); cout << "TEN SACH";
    gotoxy(70, 10); cout << "NGAY MUON";
    gotoxy(85, 10); cout << "TINH TRANG";

    int cntMuon = 0;
    bool quaHan = false;
    NgayThang today = LayNgayHienTai();

    for (PTR_MuonTra p = pDG->data.mt; p != nullptr; p = p->next) {
        if (p->data.TrangThai == 0) {
            DauSach* dSach = nullptr;
            TimSachTheoMa(DS, p->data.MaSach, dSach);
            const char* ten = dSach ? dSach->TenSach : "Unknown";
            int days = SoNgay(p->data.NgayMuon, today);

            gotoxy(7, 12 + cntMuon);
            char buf[120];
            const char* ttHan = (days > HAN_MUON_NGAY) ? "QUA HAN!" : "Binh thuong";
            if (days > HAN_MUON_NGAY) quaHan = true;

            sprintf(buf, "%-4d| %-16s| %-33.33s| %02d/%02d/%04d   | %-13s", 
                    cntMuon + 1, p->data.MaSach, ten, 
                    p->data.NgayMuon.Ngay, p->data.NgayMuon.Thang, p->data.NgayMuon.Nam, ttHan);
            cout << buf;
            cntMuon++;
        }
    }

    gotoxy(5, 18); setColor(14, 0);
    cout << "CHON THAO TAC: [1] Muon sach   [2] Tra sach   [ESC] Quay lai: ";
    setColor(15, 0);
    int opt = _getch();

    if (opt == '1') {
        if (pDG->data.TrangThai == 0) {
            gotoxy(5, 20); setColor(12, 0); cout << "(!) The bi khoa, khong duoc phep muon!";
            Sleep(1500); return;
        }
        if (quaHan) {
            gotoxy(5, 20); setColor(12, 0); cout << "(!) Doc gia giu sach qua 7 ngay, bi chan muon!";
            Sleep(1500); return;
        }
        if (cntMuon >= MAX_MUON_SACH) {
            gotoxy(5, 20); setColor(12, 0); cout << "(!) Da muon toi da 3 cuon sach!";
            Sleep(1500); return;
        }

        gotoxy(5, 20); cout << "Nhap Ma Sach muon muon: ";
        char maSach[30]; cin.getline(maSach, 30);

        DauSach* dauSach = nullptr;
        ThongTinSach* sach = TimSachTheoMa(DS, maSach, dauSach);
        if (!sach) {
            gotoxy(5, 21); setColor(12, 0); cout << "(!) Ma sach khong ton tai!";
            Sleep(1200); return;
        }
        if (sach->TrangThai != 0) {
            gotoxy(5, 21); setColor(12, 0); cout << "(!) Sach khong co san de cho muon!";
            Sleep(1200); return;
        }

        sach->TrangThai = 1;
        dauSach->SoLuotMuon++;
        PTR_MuonTra nMT = new DS_MuonTra;
        strcpy(nMT->data.MaSach, maSach);
        nMT->data.NgayMuon = today;
        nMT->data.NgayTra = NgayThang(0, 0, 0);
        nMT->data.TrangThai = 0;
        nMT->next = pDG->data.mt;
        pDG->data.mt = nMT;

        gotoxy(5, 22); setColor(10, 0); cout << "=> CHO MUON THANH CONG!";
        Sleep(1200);
    } else if (opt == '2') { // [C�U G]: Nghi?p v? Tr? S�ch
        if (cntMuon == 0) {
            gotoxy(5, 20); setColor(12, 0); cout << "(!) Doc gia khong giu cuon sach nao!";
            Sleep(1200); return;
        }
        gotoxy(5, 20); cout << "Nhap Ma Sach can tra: ";
        char maSach[30]; cin.getline(maSach, 30);

        PTR_MuonTra curMT = nullptr;
        for (PTR_MuonTra p = pDG->data.mt; p != nullptr; p = p->next) {
            if (strcmp(p->data.MaSach, maSach) == 0 && p->data.TrangThai == 0) {
                curMT = p; break;
            }
        }
        if (!curMT) {
            gotoxy(5, 21); setColor(12, 0); cout << "(!) Doc gia khong dang muon cuon sach nay!";
            Sleep(1200); return;
        }

        DauSach* dauSach = nullptr;
        ThongTinSach* sach = TimSachTheoMa(DS, maSach, dauSach);

        gotoxy(5, 21); cout << "Hinh thuc: [1] Tra binh thuong   [2] Lam mat sach: ";
        int ht = _getch();
        curMT->data.NgayTra = today;
        if (ht == '2') {
            curMT->data.TrangThai = 2; // L�m m?t
            if (sach) sach->TrangThai = 2; // Thanh l�
        } else {
            curMT->data.TrangThai = 1; // �� tr?
            if (sach) sach->TrangThai = 0; // Tr? l?i cho mu?n du?c
        }
        gotoxy(5, 23); setColor(10, 0); cout << "=> GHI NHAN TRA SACH THANH CONG!";
        Sleep(1200);
    }
}

// ===================== MH 4: top 10 sach  =====================

struct QuaHanRecord {
    int MaThe;
    char HoTen[60];
    char MaSach[30];
    NgayThang NgayMuon;
    int SoNgayTre;
};

void DuyetQuaHan(PTR_TheDocGia root, QuaHanRecord arr[], int& count, NgayThang today) {
    if (root == nullptr) return;
    DuyetQuaHan(root->left, arr, count, today);

    for (PTR_MuonTra p = root->data.mt; p != nullptr; p = p->next) {
        if (p->data.TrangThai == 0) {
            int days = SoNgay(p->data.NgayMuon, today);
            if (days > HAN_MUON_NGAY) {
                arr[count].MaThe = root->data.MaThe;
                sprintf(arr[count].HoTen, "%s %s", root->data.Ho, root->data.Ten);
                strcpy(arr[count].MaSach, p->data.MaSach);
                arr[count].NgayMuon = p->data.NgayMuon;
                arr[count].SoNgayTre = days - HAN_MUON_NGAY;
                count++;
            }
        }
    }

    DuyetQuaHan(root->right, arr, count, today);
}

void viewQuaHan(PTR_TheDocGia root) {
    system("cls");
    drawBanner(25, 2, "DANH SACH DOC GIA MUON SACH QUA HAN (GIAM DAN)");

    QuaHanRecord arr[1000];
    int count = 0;
    NgayThang today = LayNgayHienTai();
    DuyetQuaHan(root, arr, count, today);

    for (int i = 0; i < count - 1; ++i) {
        int maxIdx = i;
        for (int j = i + 1; j < count; ++j) {
            if (arr[j].SoNgayTre > arr[maxIdx].SoNgayTre) maxIdx = j;
        }
        if (maxIdx != i) swap(arr[i], arr[maxIdx]);
    }

    int colW[] = { 6, 12, 28, 15, 15, 15 };
    drawTableFrame(5, 5, colW, 6, 10);
    gotoxy(7, 6); cout << "STT";
    gotoxy(15, 6); cout << "MA THE";
    gotoxy(28, 6); cout << "HO VA TEN";
    gotoxy(57, 6); cout << "MA SACH";
    gotoxy(73, 6); cout << "NGAY MUON";
    gotoxy(89, 6); cout << "NGAY TRE";

    int top = min(count, 10);
    for (int i = 0; i < top; ++i) {
        gotoxy(7, 8 + i);
        char buf[120];
        sprintf(buf, "%-4d| %-10d| %-26.26s| %-13s| %02d/%02d/%04d   | %-13d", 
                i + 1, arr[i].MaThe, arr[i].HoTen, arr[i].MaSach, 
                arr[i].NgayMuon.Ngay, arr[i].NgayMuon.Thang, arr[i].NgayMuon.Nam, arr[i].SoNgayTre);
        cout << buf;
    }

    drawBottomBar(20);
    _getch();
}

void viewTop10(DS_DauSach& DS) {
    system("cls");
    drawBanner(25, 2, "TOP 10 SACH CO LUOT MUON NHIEU NHAT");

    DauSach* arr[MAXSACH];
    for (int i = 0; i < DS.n; ++i) arr[i] = DS.ds[i];

    for (int i = 0; i < DS.n - 1; ++i) {
        int maxIdx = i;
        for (int j = i + 1; j < DS.n; ++j) {
            if (arr[j]->SoLuotMuon > arr[maxIdx]->SoLuotMuon) maxIdx = j;
        }
        if (maxIdx != i) swap(arr[i], arr[maxIdx]);
    }

    int colW[] = { 6, 15, 35, 18, 15 };
    drawTableFrame(5, 5, colW, 5, 10);
    gotoxy(7, 6); cout << "TOP";
    gotoxy(15, 6); cout << "ISBN";
    gotoxy(35, 6); cout << "TEN SACH";
    gotoxy(70, 6); cout << "THE LOAI";
    gotoxy(88, 6); cout << "LUOT MUON";

    int top = min(DS.n, 10);
    for (int i = 0; i < top; ++i) {
        gotoxy(7, 8 + i);
        char buf[120];
        sprintf(buf, "%-4d| %-13s| %-33.33s| %-16.16s| %-13d", 
                i + 1, arr[i]->ISBN, arr[i]->TenSach, arr[i]->TheLoai, arr[i]->SoLuotMuon);
        cout << buf;
    }

    drawBottomBar(20);
    _getch();
}

// ===================== du lieu khoi tao mau =====================
void KhoiTaoDuLieuMau(DS_DauSach& DS, PTR_TheDocGia& root) {
    DauSach* ds1 = new DauSach();
    strcpy(ds1->ISBN, "INT1358");
    strcpy(ds1->TenSach, "Cau Truc Du Lieu & Giai Thuat");
    ds1->SoTrang = 380; strcpy(ds1->TacGia, "Nguyen Van A");
    ds1->NamXuatBan = 2026; strcpy(ds1->TheLoai, "Giao trinh");
    ds1->SoLuotMuon = 15;
    ThemSachChoDauSach(ds1, 4, "Ke A1-01");
    ChenDauSachCoThuTu(DS, ds1);

    DauSach* ds2 = new DauSach();
    strcpy(ds2->ISBN, "INT1306");
    strcpy(ds2->TenSach, "Lap Trinh Huong Doi Tuong");
    ds2->SoTrang = 420; strcpy(ds2->TacGia, "Tran Van B");
    ds2->NamXuatBan = 2026; strcpy(ds2->TheLoai, "Giao trinh");
    ds2->SoLuotMuon = 22;
    ThemSachChoDauSach(ds2, 3, "Ke A1-02");
    ChenDauSachCoThuTu(DS, ds2);

    DauSach* ds3 = new DauSach();
    strcpy(ds3->ISBN, "KN001");
    strcpy(ds3->TenSach, "Dac Nhan Tam");
    ds3->SoTrang = 320; strcpy(ds3->TacGia, "Dale Carnegie");
    ds3->NamXuatBan = 2024; strcpy(ds3->TheLoai, "Ky nang");
    ds3->SoLuotMuon = 8;
    ThemSachChoDauSach(ds3, 2, "Ke B2-05");
    ChenDauSachCoThuTu(DS, ds3);

    TheDocGia dg1; dg1.MaThe = 10431; strcpy(dg1.Ho, "Nguyen Van"); strcpy(dg1.Ten, "An"); strcpy(dg1.Phai, "Nam"); dg1.TrangThai = 1;
    InsertDocGia(root, dg1);

    TheDocGia dg2; dg2.MaThe = 10442; strcpy(dg2.Ho, "Le Thi"); strcpy(dg2.Ten, "Binh"); strcpy(dg2.Phai, "Nu"); dg2.TrangThai = 1;
    InsertDocGia(root, dg2);
}

// ===================== chinh thanh sang highlight =====================
int main() {
    system("mode con: cols=125 lines=32");
    srand(static_cast<unsigned int>(time(nullptr)));

    DS_DauSach DS;
    PTR_TheDocGia root = nullptr;

    LoadDauSach(DS);
    LoadTheDocGia(root);

    if (DS.n == 0 && root == nullptr) {
        KhoiTaoDuLieuMau(DS, root);
    }

    const char* menuItems[] = {
        "1. QUAN LY DAU SACH VA DANH MUC SACH  ",
        "2. QUAN LY THE DOC GIA (CAY BST)       ",
        "3. NGHIEP VU MUON - TRA SACH           ",
        "4. DANH SACH DOC GIA MUON QUA HAN      ",
        "5. TOP 10 SACH CO LUOT MUON NHIEU NHAT ",
        "6. LUU DU LIEU RA TEP TIN              ",
        "0. THOAT CHUONG TRINH                  "
    };
    int numItems = 7;
    int curChoice = 0;

    bool running = true;
    while (running) {
        showCursor(false);
        system("cls");
        drawBanner(40, 4, "HE THONG QUAN LY THU VIEN (DE TAI 3)");

        for (int i = 0; i < numItems; ++i) {
            gotoxy(42, 8 + i * 2);
            if (i == curChoice) {
                setColor(0, 14);
                cout << "  -> " << menuItems[i] << " ";
            } else {
                setColor(15, 0);
                cout << "     " << menuItems[i] << " ";
            }
        }
        setColor(15, 0);

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == KEY_UP) {
                curChoice = (curChoice - 1 + numItems) % numItems;
            } else if (ch == KEY_DOWN) {
                curChoice = (curChoice + 1) % numItems;
            }
        } else if (ch == KEY_ENTER) {
            switch (curChoice) {
                case 0: viewDauSach(DS); break;
                case 1: viewTheDocGia(root); break;
                case 2: viewMuonTra(root, DS); break;
                case 3: viewQuaHan(root); break;
                case 4: viewTop10(DS); break;
                case 5:
                    SaveDauSach(DS);
                    SaveTheDocGia(root);
                    gotoxy(42, 24); setColor(10, 0);
                    cout << "DA LUU DU LIEU VAO FILE THANH CONG!";
                    Sleep(1200);
                    break;
                case 6:
                    SaveDauSach(DS);
                    SaveTheDocGia(root);
                    running = false;
                    break;
            }
        } else if (ch == KEY_ESC) {
            running = false;
        }
    }

    system("cls");
    setColor(10, 0);
    cout << "\nDa luu toan bo du lieu va thoat chuong trinh. Tam biet!\n";
    setColor(15, 0);
    return 0;
}
