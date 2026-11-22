#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <windows.h>
#include <conio.h>

using namespace std;

template <class T>
class MangDong {
private:
    T* a;
    int n;
    int cap;

    void moRong(int need) {
        if (need <= cap) return;
        int newCap = (cap == 0 ? 8 : cap);
        while (newCap < need) newCap *= 2;
        T* b = new T[newCap];
        for (int i = 0; i < n; ++i) b[i] = a[i];
        delete[] a;
        a = b;
        cap = newCap;
    }

public:
    typedef T* iterator;
    typedef const T* const_iterator;

    MangDong() : a(nullptr), n(0), cap(0) {}
    explicit MangDong(int count) : a(nullptr), n(0), cap(0) {
        if (count > 0) {
            moRong(count);
            for (int i = 0; i < count; ++i) a[i] = T();
            n = count;
        }
    }
    MangDong(const T* first, const T* last) : a(nullptr), n(0), cap(0) {
        int count = (int)(last - first);
        if (count > 0) {
            moRong(count);
            for (int i = 0; i < count; ++i) a[i] = first[i];
            n = count;
        }
    }
    MangDong(const MangDong& other) : a(nullptr), n(0), cap(0) {
        if (other.n > 0) {
            moRong(other.n);
            for (int i = 0; i < other.n; ++i) a[i] = other.a[i];
            n = other.n;
        }
    }
    ~MangDong() { delete[] a; }

    MangDong& operator=(const MangDong& other) {
        if (this == &other) return *this;
        moRong(other.n);
        for (int i = 0; i < other.n; ++i) a[i] = other.a[i];
        n = other.n;
        return *this;
    }

    void push_back(const T& value) {
        moRong(n + 1);
        a[n++] = value;
    }
    void clear() { n = 0; }
    int size() const { return n; }
    bool empty() const { return n == 0; }
    T& operator[](int i) { return a[i]; }
    const T& operator[](int i) const { return a[i]; }
    iterator begin() { return a; }
    iterator end() { return a + n; }
    const_iterator begin() const { return a; }
    const_iterator end() const { return a + n; }
};


// ===================== MA PHIM NONG =====================
#define KEY_UP       72
#define KEY_DOWN     80
#define KEY_LEFT     75
#define KEY_RIGHT    77
#define KEY_ENTER    13
#define KEY_ESC      27
#define KEY_INSERT   82
#define KEY_DELETE   83
#define KEY_F2       60
#define KEY_F3       61
#define KEY_F5       63

// ===================== H?NG S? D? LI?U =====================
const int MAXSACH        = 10000;
const int MAX_MUON_SACH  = 3;
const int HAN_MUON_NGAY  = 7;
const int LINES_PER_PAGE = 10;
const int ROW_MSG        = 30;   // DONG HIEN THONG BAO

const char FILE_DAUSACH[] = "DAUSACH.TXT";
const char FILE_DOCGIA[]  = "THEDOCGIA.TXT";

typedef MangDong<string> Row;

// ===================== TIEN ICH CHUOI =====================
int StrICmp(const char* a, const char* b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        ++a; ++b;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

bool ContainsNoCase(const char* text, const char* key) {
    string t = text, k = key;
    for (size_t i = 0; i < t.size(); ++i) t[i] = (char)tolower((unsigned char)t[i]);
    for (size_t i = 0; i < k.size(); ++i) k[i] = (char)tolower((unsigned char)k[i]);
    return t.find(k) != string::npos;
}

void CopyStr(char* dst, size_t cap, const string& s) {
    strncpy(dst, s.c_str(), cap - 1);
    dst[cap - 1] = '\0';
}

string Trim(const string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) a++;
    while (b > a && isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

// B KY TU '|' DUNG LAM DAU PHAN CACH FILE VA KY TU DIEU KHIEN
string Sanitize(const string& s) {
    string r;
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char c = (unsigned char)s[i];
        if (c == '|') r += ' ';
        else if (c >= 32) r += (char)c;
    }
    return r;
}

void ToUpper(string& s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = (char)toupper((unsigned char)s[i]);
}

MangDong<string> Split(const string& s, char d) {
    MangDong<string> r;
    string cur;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == d) { r.push_back(cur); cur.clear(); }
        else cur += s[i];
    }
    r.push_back(cur);
    return r;
}

string Fit(const string& s, int w) {
    if (w <= 0) return "";
    string r = s.substr(0, (size_t)w);
    if ((int)r.size() < w) r.append((size_t)(w - (int)r.size()), ' ');
    return r;
}

int CalcPages(int total) {
    return (total <= 0) ? 1 : ((total - 1) / LINES_PER_PAGE + 1);
}

// ===================== DO HOA CONSOLE CO BAN =====================
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int textColor, int bgColor = 0) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)((bgColor << 4) | textColor));
}

void showCursor(bool visible) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20;
    info.bVisible = visible;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void clearArea(int x, int y, int width, int height) {
    setColor(15, 0);
    string blank((size_t)width, ' ');
    for (int i = 0; i < height; ++i) {
        gotoxy(x, y + i);
        cout << blank;
    }
}

void thongBao(const string& msg, int color, int ms = 1200) {
    clearArea(3, ROW_MSG, 100, 1);
    setColor(color, 0);
    gotoxy(3, ROW_MSG);
    cout << msg;
    Sleep(ms);
    clearArea(3, ROW_MSG, 100, 1);
}

// ===================== CAU TRUC NGAY THANG =====================
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

string DateStr(const NgayThang& d) {
    char b[32];
    snprintf(b, sizeof(b), "%02d/%02d/%04d", d.Ngay, d.Thang, d.Nam);
    return b;
}

// ===================== CAU TRUC 1: DANH MUC MUON SACH =====================
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

// ===================== CAU TRUC 2: DAU SACH =====================
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

// ===================== CAU TRUC 3: MUON TRA =====================
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

// ===================== CAU TRUC 4: THE DOC GIA (BST) =====================
struct TheDocGia {
    int MaThe;
    char Ho[40];
    char Ten[20];
    char Phai[8];
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

// ===================== GIAI PHONG BO NHO =====================
void GiaiPhongDMS(PTR_DanhMucSach& head) {
    while (head != nullptr) {
        PTR_DanhMucSach temp = head;
        head = head->next;
        delete temp;
    }
}

void GiaiPhongDauSach(DS_DauSach& DS) {
    for (int i = 0; i < DS.n; ++i) {
        GiaiPhongDMS(DS.ds[i]->dms);
        delete DS.ds[i];
        DS.ds[i] = nullptr;
    }
    DS.n = 0;
}

void GiaiPhongMT(PTR_MuonTra& head) {
    while (head != nullptr) {
        PTR_MuonTra temp = head;
        head = head->next;
        delete temp;
    }
}

void GiaiPhongDocGia(PTR_TheDocGia& root) {
    if (root == nullptr) return;
    GiaiPhongDocGia(root->left);
    GiaiPhongDocGia(root->right);
    GiaiPhongMT(root->data.mt);
    delete root;
    root = nullptr;
}

// ===================== THUAT TOAN CO SO =====================

// [CAU A]: TIM KIM DOC GIA THEO MA THE
PTR_TheDocGia TimMaThe(PTR_TheDocGia root, int MaThe) {
    if (root == nullptr || root->data.MaThe == MaThe) return root;
    if (MaThe < root->data.MaThe) return TimMaThe(root->left, MaThe);
    return TimMaThe(root->right, MaThe);
}

// [CAU A]: SINH MA THE TU DONG KHONG TRUNG
int SinhMaThe(PTR_TheDocGia root) {
    // Ma the tu dong 5 chu so, khong trung voi ma da co.
    // Neu khoang ma ngau nhien bi day, tim tuan tu de tranh lap vo han.
    for (int thu = 0; thu < 90000; ++thu) {
        int ma = rand() % 90000 + 10000;
        if (TimMaThe(root, ma) == nullptr) return ma;
    }
    for (int ma = 10000; ma <= 99999; ++ma)
        if (TimMaThe(root, ma) == nullptr) return ma;
    return -1;
}

// [CAU A]: THEM DOC GIA VAO CAY BST
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

// [CAU A]:TIM NUT THEO MANG
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

// [CAU A] XOA THE DOC GIA
bool DelDocGia(PTR_TheDocGia& p, int MaThe) {
    if (p == nullptr) return false;
    if (MaThe < p->data.MaThe) return DelDocGia(p->left, MaThe);
    if (MaThe > p->data.MaThe) return DelDocGia(p->right, MaThe);

    // con sach dang muon thi khong cho xoa
    for (PTR_MuonTra cur = p->data.mt; cur != nullptr; cur = cur->next) {
        if (cur->data.TrangThai == 0) return false;
    }

    // Giai phong lich su muon cua nut
    GiaiPhongMT(p->data.mt);

    PTR_TheDocGia temp = p;
    if (p->left == nullptr) { p = p->right; delete temp; }
    else if (p->right == nullptr) { p = p->left; delete temp; }
    else TimLop(p, p->right);   // chep du lieu nut the mang vao p roi xoa
    return true;
}

// [CAU B] CHUYEN CAY SANG MANG CON TRO
void ChuyenCayVaoMang(PTR_TheDocGia root, MangDong<TheDocGia*>& arr) {
    if (root == nullptr) return;
    ChuyenCayVaoMang(root->left, arr);
    arr.push_back(&(root->data));
    ChuyenCayVaoMang(root->right, arr);
}

// [CAU C] DEM SO CUON SACH CUA 1 DAU SACH
int DemSoSachCuaDauSach(PTR_DanhMucSach head) {
    int dem = 0;
    for (PTR_DanhMucSach p = head; p != nullptr; p = p->next) dem++;
    return dem;
}

int TimDauSachTheoISBN(const DS_DauSach& DS, const char* isbn) {
    for (int i = 0; i < DS.n; ++i) {
        if (StrICmp(DS.ds[i]->ISBN, isbn) == 0) return i;
    }
    return -1;
}

// [CAU C] CHEN DAU SACH 
bool ChenDauSachCoThuTu(DS_DauSach& DS, DauSach* dsMoi) {
    if (DS.n >= MAXSACH) return false;
    int pos = DS.n;
    while (pos > 0 && StrICmp(DS.ds[pos - 1]->TenSach, dsMoi->TenSach) > 0) {
        DS.ds[pos] = DS.ds[pos - 1];
        pos--;
    }
    DS.ds[pos] = dsMoi;
    DS.n++;
    return true;
}

// [CAU C] TU DONG DANH MA SACH
void ThemSachChoDauSach(DauSach* ds, int soLuongThem, const char* viTri) {
    int stt = DemSoSachCuaDauSach(ds->dms) + 1;
    PTR_DanhMucSach tail = ds->dms;
    while (tail != nullptr && tail->next != nullptr) tail = tail->next;

    for (int i = 0; i < soLuongThem; ++i) {
        PTR_DanhMucSach p = new DanhMucSach;
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "%s_%d", ds->ISBN, stt++);
        strncpy(p->data.MaSach, tmp, sizeof(p->data.MaSach) - 1);
        p->data.MaSach[sizeof(p->data.MaSach) - 1] = '\0';
        p->data.TrangThai = 0;
        strncpy(p->data.ViTri, viTri, sizeof(p->data.ViTri) - 1);
        p->data.ViTri[sizeof(p->data.ViTri) - 1] = '\0';
        p->next = nullptr;

        if (tail == nullptr) ds->dms = p;
        else tail->next = p;
        tail = p;
    }
}

void LayISBNTuMaSach(const char* maSach, char* isbnOut, int cap) {
    int i = 0;
    while (maSach[i] != '_' && maSach[i] != '\0' && i < cap - 1) {
        isbnOut[i] = maSach[i];
        i++;
    }
    isbnOut[i] = '\0';
}

ThongTinSach* TimSachTheoMa(const DS_DauSach& DS, const char* maSach, DauSach*& dauSachChua) {
    char isbn[20];
    LayISBNTuMaSach(maSach, isbn, 20);
    int idx = TimDauSachTheoISBN(DS, isbn);
    if (idx == -1) return nullptr;
    dauSachChua = DS.ds[idx];
    for (PTR_DanhMucSach p = dauSachChua->dms; p != nullptr; p = p->next) {
        if (StrICmp(p->data.MaSach, maSach) == 0) return &(p->data);
    }
    return nullptr;
}

const char* TenTrangThaiSach(int t) {
    if (t == 0) return "Cho muon duoc";
    if (t == 1) return "Da co nguoi muon";
    return "Da thanh ly";
}

// ===================== LUU & DOC FILE =====================
bool SaveDauSach(const DS_DauSach& DS) {
    ofstream out(FILE_DAUSACH);
    if (!out.is_open()) return false;
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
    return !out.fail();
}

void LoadDauSach(DS_DauSach& DS) {
    ifstream in(FILE_DAUSACH);
    if (!in.is_open()) return;
    string line;
    if (!getline(in, line)) return;
    int soDS = atoi(Trim(line).c_str());

    for (int i = 0; i < soDS; ++i) {
        if (!getline(in, line)) break;
        MangDong<string> f = Split(line, '|');
        if (f.size() < 8) break;           // file h?ng -> d?ng

        DauSach* p = new DauSach();
        CopyStr(p->ISBN, sizeof(p->ISBN), f[0]);
        CopyStr(p->TenSach, sizeof(p->TenSach), f[1]);
        p->SoTrang    = atoi(f[2].c_str());
        CopyStr(p->TacGia, sizeof(p->TacGia), f[3]);
        p->NamXuatBan = atoi(f[4].c_str());
        CopyStr(p->TheLoai, sizeof(p->TheLoai), f[5]);
        p->SoLuotMuon = atoi(f[6].c_str());
        int cntDMS    = atoi(f[7].c_str());

        PTR_DanhMucSach tail = nullptr;
        for (int j = 0; j < cntDMS; ++j) {
            if (!getline(in, line)) break;
            MangDong<string> g = Split(line, '|');
            if (g.size() < 3) continue;
            PTR_DanhMucSach nodeS = new DanhMucSach;
            CopyStr(nodeS->data.MaSach, sizeof(nodeS->data.MaSach), g[0]);
            nodeS->data.TrangThai = atoi(g[1].c_str());
            CopyStr(nodeS->data.ViTri, sizeof(nodeS->data.ViTri), g[2]);
            nodeS->next = nullptr;
            if (tail == nullptr) p->dms = nodeS; else tail->next = nodeS;
            tail = nodeS;
        }
        if (!ChenDauSachCoThuTu(DS, p)) {
            GiaiPhongDMS(p->dms);
            delete p;
        }
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

bool SaveTheDocGia(PTR_TheDocGia root) {
    ofstream out(FILE_DOCGIA);
    if (!out.is_open()) return false;
    out << DemSoDocGia(root) << "\n";
    GhiDocGiaNLR(root, out);
    out.close();
    return !out.fail();
}

void LoadTheDocGia(PTR_TheDocGia& root) {
    ifstream in(FILE_DOCGIA);
    if (!in.is_open()) return;
    string line;
    if (!getline(in, line)) return;
    int soDG = atoi(Trim(line).c_str());

    for (int i = 0; i < soDG; ++i) {
        if (!getline(in, line)) break;
        MangDong<string> f = Split(line, '|');
        if (f.size() < 6) break;

        TheDocGia dg;
        dg.MaThe = atoi(f[0].c_str());
        CopyStr(dg.Ho, sizeof(dg.Ho), f[1]);
        CopyStr(dg.Ten, sizeof(dg.Ten), f[2]);
        CopyStr(dg.Phai, sizeof(dg.Phai), f[3]);
        dg.TrangThai = atoi(f[4].c_str());
        int cntMT = atoi(f[5].c_str());
        dg.mt = nullptr;

        PTR_MuonTra tail = nullptr;
        for (int j = 0; j < cntMT; ++j) {
            if (!getline(in, line)) break;
            MangDong<string> g = Split(line, '|');
            if (g.size() < 8) continue;
            PTR_MuonTra nodeMT = new DS_MuonTra;
            CopyStr(nodeMT->data.MaSach, sizeof(nodeMT->data.MaSach), g[0]);
            nodeMT->data.NgayMuon = NgayThang(atoi(g[1].c_str()), atoi(g[2].c_str()), atoi(g[3].c_str()));
            nodeMT->data.NgayTra  = NgayThang(atoi(g[4].c_str()), atoi(g[5].c_str()), atoi(g[6].c_str()));
            nodeMT->data.TrangThai = atoi(g[7].c_str());
            nodeMT->next = nullptr;
            if (tail == nullptr) dg.mt = nodeMT; else tail->next = nodeMT;
            tail = nodeMT;
        }
        if (!InsertDocGia(root, dg)) GiaiPhongMT(dg.mt);   // trï¿½ng mï¿½ -> b?
    }
    in.close();
}

// ===================== KHUNG GIAO DIEN & VA BANG =====================
void drawBanner(int x, int y, const char* title) {
    setColor(0, 14);
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
    gotoxy(x - 2, y + 6);  cout << "UP    : LEN";
    gotoxy(x - 2, y + 8);  cout << "DOWN  : XUONG";
    gotoxy(x - 2, y + 10); cout << "LEFT  : TRAI";
    gotoxy(x - 2, y + 12); cout << "RIGHT : PHAI";
}

void drawPageInfo(int x, int y, int cur, int total) {
    gotoxy(x, y);
    setColor(10, 0);
    cout << "Trang : " << cur << "/" << total << "    ";
    setColor(15, 0);
}

struct Hint { const char* key; const char* text; };

void drawHints(int y, const Hint h[], int n) {
    clearArea(0, y, 112, 1);
    gotoxy(3, y);
    for (int i = 0; i < n; ++i) {
        setColor(12, 15); cout << " " << h[i].key << " ";
        setColor(15, 0);  cout << ": " << h[i].text << "  ";
    }
}

void drawRow(int x, int y, const int w[], int n, const Row& cells, int fg, int bg) {
    gotoxy(x, y);
    setColor(14, bg); cout << '|';
    for (int c = 0; c < n; ++c) {
        string txt = (c < (int)cells.size()) ? cells[c] : string();
        setColor(fg, bg);
        cout << ' ' << Fit(txt, w[c] - 1);
        setColor(14, bg);
        cout << '|';
    }
    setColor(15, 0);
}

void drawTableFrame(int x, int y, const int w[], int n, int rows) {
    string line = "+";
    for (int c = 0; c < n; ++c) line += string((size_t)w[c], '-') + "+";

    setColor(14, 0);
    gotoxy(x, y);            cout << line;
    gotoxy(x, y + 2);        cout << line;
    gotoxy(x, y + rows + 3); cout << line;

    Row empty((size_t)n);
    drawRow(x, y + 1, w, n, empty, 15, 0);
    for (int r = 0; r < rows; ++r) drawRow(x, y + 3 + r, w, n, empty, 15, 0);
    setColor(15, 0);
}

void drawBody(int x, int y, const int w[], int n, int total, int start, int perPage, int sel,
              const function<void(int, Row&)>& getRow) {
    Row cells((size_t)n);
    for (int i = 0; i < perPage; ++i) {
        int idx = start + i;
        if (idx < total) {
            for (int c = 0; c < n; ++c) cells[c].clear();
            getRow(idx, cells);
            if (i == sel) drawRow(x, y + i, w, n, cells, 0, 11);
            else          drawRow(x, y + i, w, n, cells, 15, 0);
        } else {
            for (int c = 0; c < n; ++c) cells[c].clear();
            drawRow(x, y + i, w, n, cells, 15, 0);
        }
    }
}

// ===================== NHAP LIEU AN TOAN =====================
void inputPrompt(int x, int y, const string& label) {
    clearArea(x, y, 90, 1);
    gotoxy(x, y);
    setColor(14, 0);
    cout << label;
    setColor(15, 0);
}

bool readLine(string& out) {
    if (!getline(cin, out)) {
        cin.clear();
        out.clear();
        return false;
    }
    return true;
}

// Nhap chuoi. allowEmpty = true: cho phep 
void NhapChuoi(int x, int y, const string& label, string& out, size_t maxLen, bool allowEmpty) {
    while (true) {
        inputPrompt(x, y, label);
        string s;
        readLine(s);
        s = Trim(Sanitize(s));
        if (s.empty() && !allowEmpty) { thongBao("(!) Khong duoc de trong!", 12, 800); continue; }
        if (s.size() > maxLen) {
            thongBao("(!) Toi da " + to_string(maxLen) + " ky tu!", 12, 900);
            continue;
        }
        out = s;
        return;
    }
}

// Nhap so nguyen trong [lo, hi]. Tra ve false neu nguoi dung da trung va allowCancel = true
bool NhapSo(int x, int y, const string& label, int& val, int lo, int hi, bool allowCancel) {
    while (true) {
        inputPrompt(x, y, label);
        string s;
        readLine(s);
        s = Trim(s);
        if (s.empty()) {
            if (allowCancel) return false;
            thongBao("(!) Khong duoc de trong!", 12, 800);
            continue;
        }
        bool ok = s.size() <= 9;
        for (size_t i = 0; i < s.size() && ok; ++i) if (!isdigit((unsigned char)s[i])) ok = false;
        if (ok) {
            int v = atoi(s.c_str());
            if (v >= lo && v <= hi) { val = v; return true; }
        }
        thongBao("(!) Vui long nhap so tu " + to_string(lo) + " den " + to_string(hi) + "!", 12, 1000);
    }
}

// Nhap phai: Nam / Nu. Tra ve false neu da trung (allowEmpty)
bool NhapPhai(int x, int y, const string& label, char* dst, bool allowEmpty) {
    while (true) {
        inputPrompt(x, y, label);
        string s;
        readLine(s);
        s = Trim(s);
        if (s.empty() && allowEmpty) return false;
        if (StrICmp(s.c_str(), "nam") == 0) { strcpy(dst, "Nam"); return true; }
        if (StrICmp(s.c_str(), "nu") == 0 || s == "ná»¯" || s == "Ná»¯" || s == "Ná»®") {
            strcpy(dst, "Ná»¯");
            return true;
        }
        thongBao("(!) Chi duoc nhap Nam hoac Nu/Nu co dau!", 12, 900);
    }
}

// Hoi Y/N
bool HoiYN(int x, int y, const string& msg) {
    clearArea(x, y, 95, 1);
    gotoxy(x, y);
    setColor(12, 0);
    cout << msg << " (Y/N): ";
    setColor(15, 0);
    int c = _getch();
    return (c == 'y' || c == 'Y');
}

// Qua trang khi in danh sach dai
void CheckPage(int& y, const char* title) {
    if (y >= 28) {
        gotoxy(5, 29); setColor(10, 0);
        cout << "Nhan phim bat ky de xem tiep...";
        _getch();
        system("cls");
        drawBanner(20, 2, title);
        y = 5;
    }
}

// ===================== BANG CHO XEM (cï¿½ phï¿½n trang) =====================
void viewBang(const char* title, const string& subtitle, const int colW[], int nc,
              const char* const headers[], const MangDong<Row>& rows) {
    const int X = 5, Y = 5;
    int total = (int)rows.size();
    int totalPages = CalcPages(total);
    int curPage = 1;

    showCursor(false);
    system("cls");
    drawBanner(25, 2, title);
    if (!subtitle.empty()) { setColor(11, 0); gotoxy(X, 3); cout << subtitle; }
    drawTableFrame(X, Y, colW, nc, LINES_PER_PAGE);
    Row hd;
    for (int c = 0; c < nc; ++c) hd.push_back(headers[c]);
    drawRow(X, Y + 1, colW, nc, hd, 15, 0);

    Hint h[] = { {"ESC", "Quay lai"}, {"<  >", "Chuyen trang"} };
    drawHints(21, h, 2);

    while (true) {
        int start = (curPage - 1) * LINES_PER_PAGE;
        drawBody(X, Y + 3, colW, nc, total, start, LINES_PER_PAGE, -1,
                 [&](int idx, Row& c) { c = rows[idx]; });
        if (total == 0) { setColor(8, 0); gotoxy(X + 2, Y + 3); cout << "(Khong co du lieu)"; setColor(15, 0); }
        drawPageInfo(X, Y + LINES_PER_PAGE + 4, curPage, totalPages);

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            if (ch == KEY_LEFT && curPage > 1) curPage--;
            else if (ch == KEY_RIGHT && curPage < totalPages) curPage++;
        } else if (ch == KEY_ESC || ch == KEY_ENTER) {
            return;
        }
    }
}

// ===================== MAN HINH 1: QUAN LI DAU SACH (CAU C, D, E) =====================
void viewDanhMucSach(DauSach* d) {
    MangDong<Row> rows;
    int stt = 1;
    for (PTR_DanhMucSach p = d->dms; p != nullptr; p = p->next) {
        Row r;
        r.push_back(to_string(stt++));
        r.push_back(p->data.MaSach);
        r.push_back(TenTrangThaiSach(p->data.TrangThai));
        r.push_back(p->data.ViTri);
        rows.push_back(r);
    }
    int colW[] = { 6, 20, 25, 25 };
    const char* const hd[] = { "STT", "MA SACH", "TRANG THAI", "VI TRI" };
    string sub = string("Ten sach: ") + d->TenSach + " | ISBN: " + d->ISBN;
    viewBang("DANH MUC SACH CON THUOC DAU SACH", sub, colW, 4, hd, rows);
}

void viewDauSach(DS_DauSach& DS) {
    int colW[] = { 6, 12, 35, 10, 10, 18 };
    const int nc = 6;
    Row header;
    header.push_back("STT");     header.push_back("ISBN");   header.push_back("TEN SACH");
    header.push_back("SO TRANG"); header.push_back("NAM XB"); header.push_back("THE LOAI");

    Hint hints[] = { {"ESC", "Exit"}, {"ENTER", "Xem DMS"}, {"F2", "Save"}, {"INS", "Them"},
                     {"DEL", "Xoa"}, {"F3", "Tim ten"}, {"F5", "The Loai"} };

    auto drawStatic = [&]() {
        showCursor(false);
        system("cls");
        drawBanner(25, 2, "DANH SACH DAU SACH THU VIEN (MANG CON TRO)");
        drawTableFrame(3, 4, colW, nc, LINES_PER_PAGE);
        drawRow(3, 5, colW, nc, header, 15, 0);
        drawNavigationGuide(104, 8);
        drawHints(19, hints, 7);
    };

    auto getRow = [&](int idx, Row& c) {
        c[0] = to_string(idx + 1);
        c[1] = DS.ds[idx]->ISBN;
        c[2] = DS.ds[idx]->TenSach;
        c[3] = to_string(DS.ds[idx]->SoTrang);
        c[4] = to_string(DS.ds[idx]->NamXuatBan);
        c[5] = DS.ds[idx]->TheLoai;
    };

    int curPage = 1, curRow = 0;
    drawStatic();

    while (true) {
        int total = DS.n;
        int totalPages = CalcPages(total);
        if (curPage > totalPages) curPage = totalPages;
        if (curPage < 1) curPage = 1;
        int startIdx = (curPage - 1) * LINES_PER_PAGE;
        int itemsOnPage = max(0, min(LINES_PER_PAGE, total - startIdx));
        if (itemsOnPage == 0) curRow = 0;
        else if (curRow >= itemsOnPage) curRow = itemsOnPage - 1;

        drawBody(3, 7, colW, nc, total, startIdx, LINES_PER_PAGE, (itemsOnPage > 0 ? curRow : -1), getRow);
        drawPageInfo(102, 22, curPage, totalPages);

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case KEY_UP:
                    if (curRow > 0) curRow--;
                    else if (curPage > 1) { curPage--; curRow = LINES_PER_PAGE - 1; }
                    break;
                case KEY_DOWN:
                    if (curRow < itemsOnPage - 1) curRow++;
                    else if (curPage < totalPages) { curPage++; curRow = 0; }
                    break;
                case KEY_LEFT:
                    if (curPage > 1) { curPage--; curRow = 0; }
                    break;
                case KEY_RIGHT:
                    if (curPage < totalPages) { curPage++; curRow = 0; }
                    break;

                case KEY_INSERT: { // [CAU C]: THEM DAU SACH MOI KIEM TRA TRUNG ISBN
                    if (DS.n >= MAXSACH) { thongBao("(!) Danh sach dau sach da day!", 12); break; }
                    showCursor(true);
                    clearArea(3, 21, 95, 10);
                    setColor(14, 0);
                    gotoxy(3, 21); cout << "--- NHAP DAU SACH MOI ---";

                    string isbn;
                    while (true) {
                        NhapChuoi(3, 22, "ISBN: ", isbn, 19, false);
                        ToUpper(isbn);
                        if (isbn.find('_') != string::npos || isbn.find(' ') != string::npos) {
                            thongBao("(!) ISBN khong duoc chua dau cach hoac dau '_'!", 12, 1100);
                            continue;
                        }
                        break;
                    }
                    if (TimDauSachTheoISBN(DS, isbn.c_str()) != -1) {
                        showCursor(false);
                        thongBao("(!) Ma ISBN nay da ton tai trong he thong!", 12, 1300);
                        clearArea(3, 21, 95, 10);
                        break;
                    }

                    DauSach* p = new DauSach();
                    CopyStr(p->ISBN, sizeof(p->ISBN), isbn);
                    string s; int v;
                    NhapChuoi(3, 23, "Ten sach: ", s, 59, false);  CopyStr(p->TenSach, sizeof(p->TenSach), s);
                    NhapChuoi(3, 24, "Tac gia: ", s, 39, false);   CopyStr(p->TacGia, sizeof(p->TacGia), s);
                    NhapSo(3, 25, "So trang: ", v, 1, 100000, false);  p->SoTrang = v;
                    NhapSo(3, 26, "Nam xuat ban: ", v, 1000, LayNgayHienTai().Nam, false);  p->NamXuatBan = v;
                    NhapChuoi(3, 27, "The loai: ", s, 29, false);  CopyStr(p->TheLoai, sizeof(p->TheLoai), s);
                    int soLuong;
                    NhapSo(3, 28, "So luong sach: ", soLuong, 1, 1000, false);
                    string viTri;
                    NhapChuoi(3, 29, "Vi tri ke: ", viTri, 29, false);

                    ThemSachChoDauSach(p, soLuong, viTri.c_str());
                    ChenDauSachCoThuTu(DS, p);

                    int pos = TimDauSachTheoISBN(DS, p->ISBN);
                    if (pos >= 0) { curPage = pos / LINES_PER_PAGE + 1; curRow = pos % LINES_PER_PAGE; }

                    showCursor(false);
                    clearArea(3, 21, 95, 10);
                    thongBao("=> Them dau sach thanh cong! (nho nhan F2 de luu)", 10, 1000);
                    break;
                }

                case KEY_DELETE: { // XOA DAU SACH (CHI KHI KHONG CUON NAO DANG DUOC MUON)
                    if (itemsOnPage == 0) break;
                    int idx = startIdx + curRow;
                    DauSach* d = DS.ds[idx];
                    bool dangMuon = false;
                    for (PTR_DanhMucSach q = d->dms; q != nullptr; q = q->next)
                        if (q->data.TrangThai == 1) { dangMuon = true; break; }
                    if (dangMuon) {
                        thongBao("(!) Dau sach co cuon dang duoc muon, khong the xoa!", 12, 1400);
                        break;
                    }
                    if (HoiYN(3, 21, string("Xoa dau sach ") + d->ISBN + "?")) {
                        GiaiPhongDMS(d->dms);
                        delete d;
                        for (int i = idx; i < DS.n - 1; ++i) DS.ds[i] = DS.ds[i + 1];
                        DS.ds[DS.n - 1] = nullptr;
                        DS.n--;
                        clearArea(3, 21, 95, 1);
                        thongBao("=> Da xoa dau sach!", 10, 900);
                    }
                    clearArea(3, 21, 95, 1);
                    break;
                }

                case KEY_F2:
                    if (SaveDauSach(DS)) thongBao("Da luu vao file DAUSACH.TXT thanh cong!", 10, 900);
                    else thongBao("(!) Khong ghi duoc file DAUSACH.TXT!", 12, 1200);
                    break;

                case KEY_F3: { // [CAU E]: TIM THONG TIN SACH THEO TEN SACH
                    showCursor(true);
                    clearArea(3, 21, 95, 10);
                    setColor(14, 0);
                    gotoxy(3, 21); cout << "--- TIM THONG TIN SACH THEO TEN ---";
                    string kw;
                    NhapChuoi(3, 22, "Nhap ten sach can tim (Enter de huy): ", kw, 59, true);
                    showCursor(false);
                    if (kw.empty()) { clearArea(3, 21, 95, 10); break; }

                    const char* TITLE = "KET QUA TIM KIEM THONG TIN SACH THEO TEN";
                    system("cls");
                    drawBanner(20, 2, TITLE);
                    int y = 5;
                    bool found = false;
                    for (int i = 0; i < DS.n; ++i) {
                        if (!ContainsNoCase(DS.ds[i]->TenSach, kw.c_str())) continue;
                        found = true;
                        CheckPage(y, TITLE);
                        setColor(11, 0);
                        gotoxy(5, y++);
                        cout << "[" << DS.ds[i]->ISBN << "] " << DS.ds[i]->TenSach
                             << " | Tac gia: " << DS.ds[i]->TacGia
                             << " | Nam XB: " << DS.ds[i]->NamXuatBan
                             << " | The loai: " << DS.ds[i]->TheLoai;
                        setColor(15, 0);
                        for (PTR_DanhMucSach q = DS.ds[i]->dms; q != nullptr; q = q->next) {
                            CheckPage(y, TITLE);
                            gotoxy(8, y++);
                            cout << "-> Ma sach: " << left << setw(15) << q->data.MaSach
                                 << " | Trang thai: " << setw(18) << TenTrangThaiSach(q->data.TrangThai)
                                 << " | Vi tri: " << q->data.ViTri;
                        }
                        y++;
                    }
                    if (!found) {
                        gotoxy(5, y++); setColor(12, 0); cout << "(!) Khong tim thay sach nao phu hop!";
                    }
                    gotoxy(5, y + 1); setColor(10, 0); cout << "Nhan phim bat ky de quay lai...";
                    _getch();
                    drawStatic();
                    break;
                }

                case KEY_F5: { // CAU D: IN DAU SACH THEO TUNG THE LOAI
                    const char* TITLE = "IN DAU SACH THEO TUNG THE LOAI (TEN TANG DAN)";
                    system("cls");
                    drawBanner(25, 2, TITLE);
                    int y = 5;

                    MangDong<string> theLoai;
                    for (int i = 0; i < DS.n; ++i) {
                        bool daCo = false;
                        for (size_t j = 0; j < theLoai.size(); ++j)
                            if (StrICmp(DS.ds[i]->TheLoai, theLoai[j].c_str()) == 0) { daCo = true; break; }
                        if (!daCo) theLoai.push_back(DS.ds[i]->TheLoai);
                    }
                    sort(theLoai.begin(), theLoai.end(),
                         [](const string& a, const string& b) { return StrICmp(a.c_str(), b.c_str()) < 0; });

                    for (size_t t = 0; t < theLoai.size(); ++t) {
                        CheckPage(y, TITLE);
                        setColor(14, 0);
                        gotoxy(5, y++); cout << ">> THE LOAI: " << theLoai[t];
                        setColor(15, 0);
                        for (int i = 0; i < DS.n; ++i) {   // DS DA SAP XEP TANG DAN THEO TEN SACH
                            if (StrICmp(DS.ds[i]->TheLoai, theLoai[t].c_str()) != 0) continue;
                            CheckPage(y, TITLE);
                            gotoxy(7, y++);
                            cout << "- [" << DS.ds[i]->ISBN << "] " << DS.ds[i]->TenSach
                                 << " (" << DS.ds[i]->TacGia << ", " << DS.ds[i]->NamXuatBan << ")";
                        }
                        y++;
                    }
                    if (theLoai.empty()) { gotoxy(5, y++); setColor(12, 0); cout << "(!) Chua co dau sach nao!"; }
                    gotoxy(5, y); setColor(10, 0); cout << "Nhan phim bat ky de quay lai...";
                    _getch();
                    drawStatic();
                    break;
                }
            }
        } else if (ch == KEY_ESC) {
            return;
        } else if (ch == KEY_ENTER) { // XEM CHI TIET DANH MUC SACH CON (DMS)
            if (itemsOnPage > 0) {
                viewDanhMucSach(DS.ds[startIdx + curRow]);
                drawStatic();
            }
        }
    }
}

// ===================== MAN HINH 2: QUAN LI THE DOC GIA (CAU A, B) =====================
void viewTheDocGia(PTR_TheDocGia& root) {
    int colW[] = { 6, 15, 28, 15, 8, 18 };
    const int nc = 6;
    Row header;
    header.push_back("STT");  header.push_back("MA THE"); header.push_back("HO");
    header.push_back("TEN");  header.push_back("PHAI");   header.push_back("TRANG THAI");

    Hint hints[] = { {"ESC", "Exit"}, {"F2", "Save"}, {"INS", "Them"}, {"F3", "Sua"},
                     {"DEL", "Xoa"}, {"F5", "Xep Ten / Ma"} };

    showCursor(false);
    system("cls");
    drawBanner(25, 2, "DANH SACH THE DOC GIA (CAY NHI PHAN TIM KIEM BST)");
    drawTableFrame(3, 4, colW, nc, LINES_PER_PAGE);
    drawRow(3, 5, colW, nc, header, 15, 0);
    drawNavigationGuide(104, 8);
    drawHints(19, hints, 6);

    MangDong<TheDocGia*> arr;
    bool sortByName = false;

    auto rebuild = [&]() {
        arr.clear();
        ChuyenCayVaoMang(root, arr);
        if (sortByName) {
            stable_sort(arr.begin(), arr.end(), [](const TheDocGia* a, const TheDocGia* b) {
                int c = StrICmp(a->Ten, b->Ten);
                if (c != 0) return c < 0;
                return StrICmp(a->Ho, b->Ho) < 0;
            });
        }
    };

    auto getRow = [&](int idx, Row& c) {
        c[0] = to_string(idx + 1);
        c[1] = to_string(arr[idx]->MaThe);
        c[2] = arr[idx]->Ho;
        c[3] = arr[idx]->Ten;
        c[4] = arr[idx]->Phai;
        c[5] = (arr[idx]->TrangThai == 1) ? "Hoat dong" : "Bi khoa";
    };

    rebuild();
    int curPage = 1, curRow = 0;

    while (true) {
        int total = (int)arr.size();
        int totalPages = CalcPages(total);
        if (curPage > totalPages) curPage = totalPages;
        if (curPage < 1) curPage = 1;
        int startIdx = (curPage - 1) * LINES_PER_PAGE;
        int itemsOnPage = max(0, min(LINES_PER_PAGE, total - startIdx));
        if (itemsOnPage == 0) curRow = 0;
        else if (curRow >= itemsOnPage) curRow = itemsOnPage - 1;

        setColor(11, 0);
        gotoxy(3, 3);
        cout << (sortByName ? "Sap xep: TEN + HO (tang dan)   " : "Sap xep: MA THE (tang dan)     ");
        setColor(15, 0);

        drawBody(3, 7, colW, nc, total, startIdx, LINES_PER_PAGE, (itemsOnPage > 0 ? curRow : -1), getRow);
        drawPageInfo(102, 22, curPage, totalPages);

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case KEY_UP:
                    if (curRow > 0) curRow--;
                    else if (curPage > 1) { curPage--; curRow = LINES_PER_PAGE - 1; }
                    break;
                case KEY_DOWN:
                    if (curRow < itemsOnPage - 1) curRow++;
                    else if (curPage < totalPages) { curPage++; curRow = 0; }
                    break;
                case KEY_LEFT:
                    if (curPage > 1) { curPage--; curRow = 0; }
                    break;
                case KEY_RIGHT:
                    if (curPage < totalPages) { curPage++; curRow = 0; }
                    break;

                case KEY_INSERT: { // [CAU A]: THEM THE DOC GIA
                    showCursor(true);
                    clearArea(3, 21, 95, 10);
                    setColor(14, 0);
                    gotoxy(3, 21); cout << "--- THEM THE DOC GIA MOI ---";
                    TheDocGia dg;
                    dg.MaThe = SinhMaThe(root);
                    if (dg.MaThe == -1) {
                        showCursor(false);
                        thongBao("(!) Da het kho ma the co the cap!", 12, 1200);
                        break;
                    }
                    gotoxy(3, 22); cout << "Ma the (tu dong): " << dg.MaThe;
                    string s;
                    NhapChuoi(3, 23, "Ho: ", s, 39, false);  CopyStr(dg.Ho, sizeof(dg.Ho), s);
                    NhapChuoi(3, 24, "Ten: ", s, 19, false); CopyStr(dg.Ten, sizeof(dg.Ten), s);
                    NhapPhai(3, 25, "Phai (Nam/Nu): ", dg.Phai, false);
                    dg.TrangThai = 1;
                    dg.mt = nullptr;
                    int newMa = dg.MaThe;
                    InsertDocGia(root, dg);

                    rebuild();
                    for (size_t i = 0; i < arr.size(); ++i)
                        if (arr[i]->MaThe == newMa) { curPage = (int)i / LINES_PER_PAGE + 1; curRow = (int)i % LINES_PER_PAGE; break; }

                    showCursor(false);
                    clearArea(3, 21, 95, 10);
                    thongBao("=> Them doc gia thanh cong! (nho nhan F2 de luu)", 10, 1000);
                    break;
                }

                case KEY_DELETE: { // [CAU A]: XOA THE DOC GIA
                    if (itemsOnPage == 0) break;
                    int maXoa = arr[startIdx + curRow]->MaThe;
                    if (HoiYN(3, 21, "Ban co chac muon xoa the doc gia " + to_string(maXoa) + "?")) {
                        clearArea(3, 21, 95, 1);
                        if (DelDocGia(root, maXoa)) thongBao("=> Da xoa the doc gia thanh cong!", 10, 1000);
                        else thongBao("(!) Doc gia dang muon sach, khong xoa duoc!", 12, 1300);
                        rebuild();
                    }
                    clearArea(3, 21, 95, 1);
                    break;
                }

                case KEY_F3: { // [CAU A]: HIEU CHINH THE DOC GIA
                    if (itemsOnPage == 0) break;
                    TheDocGia* d = arr[startIdx + curRow];
                    showCursor(true);
                    clearArea(3, 21, 95, 10);
                    setColor(14, 0);
                    gotoxy(3, 21); cout << "--- HIEU CHINH DOC GIA MA THE: " << d->MaThe << " (Enter = giu nguyen) ---";
                    string s;
                    NhapChuoi(3, 22, string("Ho [") + d->Ho + "]: ", s, 39, true);
                    if (!s.empty()) CopyStr(d->Ho, sizeof(d->Ho), s);
                    NhapChuoi(3, 23, string("Ten [") + d->Ten + "]: ", s, 19, true);
                    if (!s.empty()) CopyStr(d->Ten, sizeof(d->Ten), s);
                    NhapPhai(3, 24, string("Phai [") + d->Phai + "] (Nam/Nu): ", d->Phai, true);
                    int tt;
                    if (NhapSo(3, 25, "Trang thai the (0: Khoa, 1: Hoat dong) [" + to_string(d->TrangThai) + "]: ", tt, 0, 1, true))
                        d->TrangThai = tt;

                    showCursor(false);
                    clearArea(3, 21, 95, 10);
                    int ma = d->MaThe;
                    rebuild();   // Tï¿½n/H? cï¿½ th? d?i -> s?p x?p l?i
                    for (size_t i = 0; i < arr.size(); ++i)
                        if (arr[i]->MaThe == ma) { curPage = (int)i / LINES_PER_PAGE + 1; curRow = (int)i % LINES_PER_PAGE; break; }
                    break;
                }

                case KEY_F2:
                    if (SaveTheDocGia(root)) thongBao("Da luu vao file THEDOCGIA.TXT thanh cong!", 10, 900);
                    else thongBao("(!) Khong ghi duoc file THEDOCGIA.TXT!", 12, 1200);
                    break;

                case KEY_F5: // CAU B: CHUYEN THE DOC GIA XEP THEO TEN+HO VA XEP MA DOC GIA
                    sortByName = !sortByName;
                    rebuild();
                    curPage = 1; curRow = 0;
                    break;
            }
        } else if (ch == KEY_ESC) {
            return;
        }
    }
}

// ===================== MAN HINH 3: MUON TRA SACH (CAU F, G, H) =====================
void viewMuonTra(PTR_TheDocGia root, DS_DauSach& DS) {
    system("cls");
    drawBanner(25, 2, "NGHIEP VU MUON - TRA SACH");
    showCursor(true);

    int maThe;
    if (!NhapSo(5, 5, "NHAP MA THE DOC GIA (Enter de quay lai): ", maThe, 1, 999999999, true)) {
        showCursor(false);
        return;
    }
    PTR_TheDocGia pDG = TimMaThe(root, maThe);
    if (!pDG) {
        showCursor(false);
        thongBao("(!) KHONG TIM THAY MA THE!", 12, 1200);
        return;
    }

    int colMT[] = { 6, 18, 35, 15, 15 };
    const int nc = 5;
    const int rowsMT = MAX_MUON_SACH + 2;
    Row header;
    header.push_back("STT"); header.push_back("MA SACH"); header.push_back("TEN SACH");
    header.push_back("NGAY MUON"); header.push_back("TINH TRANG");

    while (true) {
        NgayThang today = LayNgayHienTai();

        // [CAU H]: DANH SACH CAC SACH DANG MUON
        MangDong<PTR_MuonTra> loans;
        bool quaHan = false;
        for (PTR_MuonTra p = pDG->data.mt; p != nullptr; p = p->next) {
            if (p->data.TrangThai != 0) continue;
            loans.push_back(p);
            if (SoNgay(p->data.NgayMuon, today) > HAN_MUON_NGAY) quaHan = true;
        }
        int cntMuon = (int)loans.size();

        showCursor(false);
        system("cls");
        drawBanner(25, 2, "NGHIEP VU MUON - TRA SACH");
        setColor(11, 0);
        gotoxy(5, 4);
        cout << "Ma the: " << pDG->data.MaThe << " | Doc gia: " << pDG->data.Ho << " " << pDG->data.Ten
             << " | Phai: " << pDG->data.Phai
             << " | Trang thai: " << (pDG->data.TrangThai == 1 ? "Hoat dong" : "BI KHOA");

        drawTableFrame(5, 6, colMT, nc, rowsMT);
        drawRow(5, 7, colMT, nc, header, 15, 0);
        drawBody(5, 9, colMT, nc, cntMuon, 0, rowsMT, -1, [&](int idx, Row& c) {
            DauSach* dSach = nullptr;
            TimSachTheoMa(DS, loans[idx]->data.MaSach, dSach);
            int days = SoNgay(loans[idx]->data.NgayMuon, today);
            c[0] = to_string(idx + 1);
            c[1] = loans[idx]->data.MaSach;
            c[2] = dSach ? dSach->TenSach : "Unknown";
            c[3] = DateStr(loans[idx]->data.NgayMuon);
            c[4] = (days > HAN_MUON_NGAY) ? "QUA HAN!" : "Binh thuong";
        });

        gotoxy(5, 16); setColor(14, 0);
        cout << "CHON THAO TAC: [1] Muon sach   [2] Tra sach   [ESC] Quay lai";
        setColor(15, 0);
        int opt = _getch();
        if (opt == KEY_ESC) { showCursor(false); return; }

        if (opt == '1') { // [CAU F] MUON SACH
            if (pDG->data.TrangThai == 0) { thongBao("(!) The bi khoa, khong duoc phep muon!", 12, 1500); continue; }
            if (quaHan) { thongBao("(!) Doc gia giu sach qua 7 ngay, bi chan muon!", 12, 1500); continue; }
            if (cntMuon >= MAX_MUON_SACH) { thongBao("(!) Da muon toi da 3 cuon sach!", 12, 1500); continue; }

            showCursor(true);
            string maSach;
            NhapChuoi(5, 18, "Nhap Ma Sach muon muon (Enter de huy): ", maSach, 29, true);
            showCursor(false);
            if (maSach.empty()) continue;
            ToUpper(maSach);

            DauSach* dauSach = nullptr;
            ThongTinSach* sach = TimSachTheoMa(DS, maSach.c_str(), dauSach);
            if (!sach) { thongBao("(!) Ma sach khong ton tai!", 12, 1200); continue; }
            if (sach->TrangThai != 0) { thongBao("(!) Sach khong co san de cho muon!", 12, 1200); continue; }

            sach->TrangThai = 1;
            dauSach->SoLuotMuon++;
            PTR_MuonTra nMT = new DS_MuonTra;
            CopyStr(nMT->data.MaSach, sizeof(nMT->data.MaSach), sach->MaSach);
            nMT->data.NgayMuon = today;
            nMT->data.NgayTra = NgayThang(0, 0, 0);
            nMT->data.TrangThai = 0;
            nMT->next = pDG->data.mt;
            pDG->data.mt = nMT;

            thongBao("=> CHO MUON THANH CONG!", 10, 1200);
        } else if (opt == '2') { // [CAU G] TRA SACH
            if (cntMuon == 0) { thongBao("(!) Doc gia khong giu cuon sach nao!", 12, 1200); continue; }

            showCursor(true);
            string maSach;
            NhapChuoi(5, 18, "Nhap Ma Sach can tra (Enter de huy): ", maSach, 29, true);
            showCursor(false);
            if (maSach.empty()) continue;
            ToUpper(maSach);

            PTR_MuonTra curMT = nullptr;
            for (size_t i = 0; i < loans.size(); ++i) {
                if (StrICmp(loans[i]->data.MaSach, maSach.c_str()) == 0) { curMT = loans[i]; break; }
            }
            if (!curMT) { thongBao("(!) Doc gia khong dang muon cuon sach nay!", 12, 1200); continue; }

            DauSach* dauSach = nullptr;
            ThongTinSach* sach = TimSachTheoMa(DS, curMT->data.MaSach, dauSach);

            gotoxy(5, 19); setColor(14, 0);
            cout << "Hinh thuc: [1] Tra binh thuong   [2] Lam mat sach   [ESC] Huy: ";
            setColor(15, 0);
            int ht = _getch();
            if (ht != '1' && ht != '2') continue;

            curMT->data.NgayTra = today;
            if (ht == '2') {
                curMT->data.TrangThai = 2;      // LAM M?T
                if (sach) sach->TrangThai = 2;  // Thanh lY
            } else {
                curMT->data.TrangThai = 1;      // DA TRA
                if (sach) sach->TrangThai = 0;  // TRA VE KHO
            }
            thongBao("=> GHI NHAN TRA SACH THANH CONG!", 10, 1200);
        }
    }
}

// ===================== MAN HINH 4: THONG KE QUA HAN & TOP 10(CAU I,J) =====================
struct QuaHanRecord {
    int MaThe;
    string HoTen;
    string MaSach;
    NgayThang NgayMuon;
    int SoNgayTre;
};

void DuyetQuaHan(PTR_TheDocGia root, MangDong<QuaHanRecord>& arr, NgayThang today) {
    if (root == nullptr) return;
    DuyetQuaHan(root->left, arr, today);

    for (PTR_MuonTra p = root->data.mt; p != nullptr; p = p->next) {
        if (p->data.TrangThai == 0) {
            int days = SoNgay(p->data.NgayMuon, today);
            if (days > HAN_MUON_NGAY) {
                QuaHanRecord r;
                r.MaThe = root->data.MaThe;
                r.HoTen = string(root->data.Ho) + " " + root->data.Ten;
                r.MaSach = p->data.MaSach;
                r.NgayMuon = p->data.NgayMuon;
                r.SoNgayTre = days - HAN_MUON_NGAY;
                arr.push_back(r);
            }
        }
    }

    DuyetQuaHan(root->right, arr, today);
}

void viewQuaHan(PTR_TheDocGia root) {
    MangDong<QuaHanRecord> arr;
    DuyetQuaHan(root, arr, LayNgayHienTai());
    stable_sort(arr.begin(), arr.end(), [](const QuaHanRecord& a, const QuaHanRecord& b) {
        return a.SoNgayTre > b.SoNgayTre;
    });

    MangDong<Row> rows;
    for (size_t i = 0; i < arr.size(); ++i) {
        Row r;
        r.push_back(to_string(i + 1));
        r.push_back(to_string(arr[i].MaThe));
        r.push_back(arr[i].HoTen);
        r.push_back(arr[i].MaSach);
        r.push_back(DateStr(arr[i].NgayMuon));
        r.push_back(to_string(arr[i].SoNgayTre));
        rows.push_back(r);
    }
    int colW[] = { 6, 12, 28, 15, 15, 15 };
    const char* const hd[] = { "STT", "MA THE", "HO VA TEN", "MA SACH", "NGAY MUON", "NGAY TRE" };
    viewBang("DANH SACH DOC GIA MUON SACH QUA HAN (GIAM DAN)", "", colW, 6, hd, rows);
}

void viewTop10(DS_DauSach& DS) {
    MangDong<DauSach*> arr(DS.ds, DS.ds + DS.n);
    stable_sort(arr.begin(), arr.end(), [](const DauSach* a, const DauSach* b) {
        return a->SoLuotMuon > b->SoLuotMuon;
    });

    MangDong<Row> rows;
    int top = min((int)arr.size(), 10);
    for (int i = 0; i < top; ++i) {
        Row r;
        r.push_back(to_string(i + 1));
        r.push_back(arr[i]->ISBN);
        r.push_back(arr[i]->TenSach);
        r.push_back(arr[i]->TheLoai);
        r.push_back(to_string(arr[i]->SoLuotMuon));
        rows.push_back(r);
    }
    int colW[] = { 6, 15, 35, 18, 15 };
    const char* const hd[] = { "TOP", "ISBN", "TEN SACH", "THE LOAI", "LUOT MUON" };
    viewBang("TOP 10 SACH CO LUOT MUON NHIEU NHAT", "", colW, 5, hd, rows);
}

// ===================== DU LIEU KHOI TAO MAU =====================
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

// ===================== MENU CHINH THANH SANG HIGHLIGHT =====================
int main() {
    system("mode con: cols=120 lines=35");
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

    // Ve menu mot lan. Khi di chuyen chi cap nhat dong cu va dong moi,
    // khong xoa toan bo man hinh bang system("cls") sau moi lan nhan phim.
    auto veMotDongMenu = [&](int i) {
        gotoxy(42, 8 + i * 2);
        if (i == curChoice) {
            setColor(0, 14);
            cout << "  -> " << menuItems[i] << " ";
        } else {
            setColor(15, 0);
            cout << "     " << menuItems[i] << " ";
        }
        setColor(15, 0);
    };

    auto veLaiMenuChinh = [&]() {
        system("cls");
        drawBanner(40, 4, "HE THONG QUAN LY THU VIEN (DE TAI 3)");
        for (int i = 0; i < numItems; ++i) veMotDongMenu(i);
        setColor(15, 0);
    };

    bool running = true;
    veLaiMenuChinh();

    while (running) {
        showCursor(false);
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            ch = _getch();
            int oldChoice = curChoice;
            if (ch == KEY_UP) {
                curChoice = (curChoice - 1 + numItems) % numItems;
            } else if (ch == KEY_DOWN) {
                curChoice = (curChoice + 1) % numItems;
            }
            if (oldChoice != curChoice) {
                veMotDongMenu(oldChoice);
                veMotDongMenu(curChoice);
            }
        } else if (ch == KEY_ENTER) {
            switch (curChoice) {
                case 0: viewDauSach(DS); veLaiMenuChinh(); break;
                case 1: viewTheDocGia(root); veLaiMenuChinh(); break;
                case 2: viewMuonTra(root, DS); veLaiMenuChinh(); break;
                case 3: viewQuaHan(root); veLaiMenuChinh(); break;
                case 4: viewTop10(DS); veLaiMenuChinh(); break;
                case 5: {
                    bool ok1 = SaveDauSach(DS);
                    bool ok2 = SaveTheDocGia(root);
                    gotoxy(30, 24);
                    if (ok1 && ok2) { setColor(10, 0); cout << "DA LUU DU LIEU VAO FILE THANH CONG!"; }
                    else            { setColor(12, 0); cout << "(!) LOI: KHONG GHI DUOC FILE!"; }
                    Sleep(1200);
                    veLaiMenuChinh();
                    break;
                }
                case 6:
                    SaveDauSach(DS);
                    SaveTheDocGia(root);
                    running = false;
                    break;
            }
        } else if (ch == KEY_ESC) {
            gotoxy(20, 24);
            clearArea(20, 24, 100, 1);
            setColor(14, 0);
            cout << "Luu truoc khi thoat? [Y] Luu & thoat  [N] Khong luu  [Khac] O lai";
            int c = _getch();
            if (c == 'y' || c == 'Y') {
                SaveDauSach(DS);
                SaveTheDocGia(root);
                running = false;
            } else if (c == 'n' || c == 'N') {
                running = false;
            } else {
                veLaiMenuChinh();
            }
        }
    }

    GiaiPhongDauSach(DS);
    GiaiPhongDocGia(root);

    system("cls");
    setColor(10, 0);
    cout << "\nDa thu hoi bo nho. Tam biet!\n";
    setColor(15, 0);
    return 0;
}