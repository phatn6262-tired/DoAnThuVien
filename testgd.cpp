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
#define KEY_F6       64

// ===================== H?NG S? D? LI?U =====================
const long long MAXSACH  = 10000;
const int MAXDOCGIA      = 10000;
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
    NgayThang d;
    d.Ngay = ltm->tm_mday;
    d.Thang = ltm->tm_mon + 1;
    d.Nam = ltm->tm_year + 1900;
    return d;
}

string DateStr(const NgayThang& d) {
    char b[32];
    snprintf(b, sizeof(b), "%02d/%02d/%04d", d.Ngay, d.Thang, d.Nam);
    return b;
}

void InNgayThang(NgayThang nt) {
    cout << nt.Ngay << "/" << nt.Thang << "/" << nt.Nam;
}

// ===================== CAU TRUC 1: DANH MUC MUON SACH =====================
struct ThongTinSach {
    string MaSach;
    int TrangThai; // 0: cho muon duoc, 1: da muon, 2: thanh ly
    string ViTri;
};

struct DanhMucSach {
    ThongTinSach data;
    DanhMucSach* next;
};
typedef DanhMucSach* PTR_DanhMucSach;

// ===================== CAU TRUC 2: DAU SACH =====================
struct DauSach {
    string ISBN, TenSach, TacGia, TheLoai;
    int SoTrang, NamXuatBan;
    PTR_DanhMucSach dms = NULL;
    int SoLuotMuon = 0; // them: phuc vu thong ke top sach (khong co trong wuanh.cpp)
};

struct DS_DauSach {
    DauSach* ds[MAXSACH];
    int n = 0;
};

// ===================== CAU TRUC 3: MUON TRA =====================
struct MuonTra {
    string MaSach;
    NgayThang NgayMuon, NgayTra;
    int TrangThai; // 0: dang muon, 1: da tra, 2: lam mat
};

struct DS_MuonTra {
    MuonTra data;
    DS_MuonTra* next;
};
typedef DS_MuonTra* PTR_MuonTra;

// ===================== CAU TRUC 4: THE DOC GIA (BST) =====================
struct TheDocGia {
    int MaThe;
    char ho[40];
    char ten[20];
    char phai[5];
    int TrangThaiThe; // 0: bi khoa, 1: hoat dong
    PTR_MuonTra mt;

    TheDocGia() {
        MaThe = 0;
        ho[0] = '\0';
        ten[0] = '\0';
        phai[0] = '\0';
        TrangThaiThe = 1;
        mt = nullptr;
    }

    TheDocGia(int _ma, const char* _ho, const char* _ten, const char* _phai, int _tt, PTR_MuonTra _mt) {
        MaThe = _ma;
        strcpy(ho, _ho);
        strcpy(ten, _ten);
        strcpy(phai, _phai);
        TrangThaiThe = _tt;
        mt = _mt;
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
PTR_TheDocGia TimMaThe(PTR_TheDocGia &root, int MaThe) {
    if (root == nullptr || root->data.MaThe == MaThe) return root;
    if (MaThe < root->data.MaThe) return TimMaThe(root->left, MaThe);
    return TimMaThe(root->right, MaThe);
}

PTR_TheDocGia SearchDocGia(PTR_TheDocGia root, int MaThe) {
    if (root == nullptr || root->data.MaThe == MaThe) return root;
    if (MaThe < root->data.MaThe) return SearchDocGia(root->left, MaThe);
    return SearchDocGia(root->right, MaThe);
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
void TimLop(DS_TheDocGia*& p, DS_TheDocGia*& q) {
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

// [CAU F] KIEM TRA DOC GIA CO SACH MUON QUA HAN KHONG
int coQuaHan(PTR_TheDocGia &p) {
    NgayThang NgayHienTai = LayNgayHienTai();
    for (PTR_MuonTra q = p->data.mt; q != NULL; q = q->next) {
        if (q->data.TrangThai == 0) {
            int tong = SoNgay(q->data.NgayMuon, NgayHienTai);
            if (tong > HAN_MUON_NGAY) return 1;
        }
    }
    return 0;
}

// [CAU B] CHUYEN CAY SANG MANG CON TRO
void chuyenCayVaoMang(PTR_TheDocGia root, TheDocGia* arr[], int& count) {
    if (root == nullptr) return;
    chuyenCayVaoMang(root->left, arr, count);
    if (count < MAXDOCGIA) arr[count++] = &(root->data);
    chuyenCayVaoMang(root->right, arr, count);
}

// [CAU C] DEM SO CUON SACH CUA 1 DAU SACH
int DemSoSachCon(PTR_DanhMucSach head) {
    int dem = 0;
    for (PTR_DanhMucSach p = head; p != nullptr; p = p->next) dem++;
    return dem;
}

int TimDauSachTheoISBN(DS_DauSach *ds, string isbn) {
    for (int i = 0; i < ds->n; ++i) {
        if (StrICmp(ds->ds[i]->ISBN.c_str(), isbn.c_str()) == 0) return i;
    }
    return -1;
}

// [CAU C] CHEN DAU SACH 
void ThemDauSach(DS_DauSach *ds, DauSach b) {
    if (ds->n >= MAXSACH) return;
    DauSach* pMoi = new DauSach;
    *pMoi = b;
    int pos = ds->n;
    while (pos > 0 && StrICmp(ds->ds[pos - 1]->TenSach.c_str(), pMoi->TenSach.c_str()) > 0) {
        ds->ds[pos] = ds->ds[pos - 1];
        pos--;
    }
    ds->ds[pos] = pMoi;
    ds->n++;
}

// [CAU C] TU DONG DANH MA SACH
void ThemSachChoDauSach(DauSach* ds, int soLuongThem, const char* viTri) {
    int stt = DemSoSachCon(ds->dms) + 1;
    PTR_DanhMucSach tail = ds->dms;
    while (tail != nullptr && tail->next != nullptr) tail = tail->next;

    for (int i = 0; i < soLuongThem; ++i) {
        PTR_DanhMucSach p = new DanhMucSach;
        p->data.MaSach = ds->ISBN + "_" + to_string(stt++);
        p->data.TrangThai = 0;
        p->data.ViTri = viTri;
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

PTR_DanhMucSach TimSach(DS_DauSach DS, string MaSach) {
    char isbn[20];
    LayISBNTuMaSach(MaSach.c_str(), isbn, 20);
    int idx = TimDauSachTheoISBN(&DS, isbn);
    if (idx == -1) return nullptr;
    for (PTR_DanhMucSach p = DS.ds[idx]->dms; p != nullptr; p = p->next) {
        if (StrICmp(p->data.MaSach.c_str(), MaSach.c_str()) == 0) return p;
    }
    return nullptr;
}

// (them) TIM DAU SACH CHUA MOT MA SACH
DauSach* TimDauSachChuaMaSach(DS_DauSach& DS, string MaSach) {
    char isbn[20];
    LayISBNTuMaSach(MaSach.c_str(), isbn, 20);
    int idx = TimDauSachTheoISBN(&DS, isbn);
    return (idx == -1) ? nullptr : DS.ds[idx];
}

void CapNhatSachMuon(PTR_MuonTra &n, string MaSach) {
    n->data.MaSach = MaSach;
    n->data.NgayMuon = LayNgayHienTai();
    n->data.NgayTra = {0, 0, 0};
    n->data.TrangThai = 0;
    n->next = NULL;
}

PTR_MuonTra TimSachDaMuon(PTR_MuonTra &p, string MaSach) {
    PTR_MuonTra q;
    PTR_MuonTra pre_q = NULL;
    for (q = p; q != NULL; q = q->next) {
        if (MaSach == q->data.MaSach) {
            pre_q = q;
            if (q->data.TrangThai == 0) {
                return q;
            }
        }
    }
    return pre_q;
}

const char* TenTrangThaiSach(int t) {
    if (t == 0) return "Cho muon duoc";
    if (t == 1) return "Da co nguoi muon";
    return "Da thanh ly";
}

string TimTenSach(const DS_DauSach &DS, string MaSach) {
    for (int i = 0; i < DS.n; i++) {
        DauSach *p = DS.ds[i];
        for (PTR_DanhMucSach q = p->dms; q != NULL; q = q->next) {
            if (q->data.MaSach == MaSach) return p->TenSach;
        }
    }
    return "Khong tim thay";
}

// ===================== LUU & DOC FILE =====================
bool SaveDauSach(const DS_DauSach& DS) {
    ofstream out(FILE_DAUSACH);
    if (!out.is_open()) return false;
    out << DS.n << "\n";
    for (int i = 0; i < DS.n; ++i) {
        int cntDMS = DemSoSachCon(DS.ds[i]->dms);
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

void ReadDauSach(DS_DauSach *DS) {
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
        p->ISBN       = f[0];
        p->TenSach    = f[1];
        p->SoTrang    = atoi(f[2].c_str());
        p->TacGia     = f[3];
        p->NamXuatBan = atoi(f[4].c_str());
        p->TheLoai    = f[5];
        p->SoLuotMuon = atoi(f[6].c_str());
        int cntDMS    = atoi(f[7].c_str());

        PTR_DanhMucSach tail = nullptr;
        for (int j = 0; j < cntDMS; ++j) {
            if (!getline(in, line)) break;
            MangDong<string> g = Split(line, '|');
            if (g.size() < 3) continue;
            PTR_DanhMucSach nodeS = new DanhMucSach;
            nodeS->data.MaSach = g[0];
            nodeS->data.TrangThai = atoi(g[1].c_str());
            nodeS->data.ViTri = g[2];
            nodeS->next = nullptr;
            if (tail == nullptr) p->dms = nodeS; else tail->next = nodeS;
            tail = nodeS;
        }
        int truoc = DS->n;
        ThemDauSach(DS, *p);
        if (DS->n == truoc) GiaiPhongDMS(p->dms);   // day -> bo
        delete p;
    }
    in.close();
}

void GhiDocGiaNLR(PTR_TheDocGia root, ofstream& out) {
    if (!root) return;
    int cntMT = 0;
    for (PTR_MuonTra p = root->data.mt; p != nullptr; p = p->next) cntMT++;
    out << root->data.MaThe << "|" << root->data.ho << "|" << root->data.ten << "|"
        << root->data.phai << "|" << root->data.TrangThaiThe << "|" << cntMT << "\n";
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
        CopyStr(dg.ho, sizeof(dg.ho), f[1]);
        CopyStr(dg.ten, sizeof(dg.ten), f[2]);
        CopyStr(dg.phai, sizeof(dg.phai), f[3]);
        dg.TrangThaiThe = atoi(f[4].c_str());
        int cntMT = atoi(f[5].c_str());
        dg.mt = nullptr;

        PTR_MuonTra tail = nullptr;
        for (int j = 0; j < cntMT; ++j) {
            if (!getline(in, line)) break;
            MangDong<string> g = Split(line, '|');
            if (g.size() < 8) continue;
            PTR_MuonTra nodeMT = new DS_MuonTra;
            nodeMT->data.MaSach = g[0];
            nodeMT->data.NgayMuon = { atoi(g[1].c_str()), atoi(g[2].c_str()), atoi(g[3].c_str()) };
            nodeMT->data.NgayTra  = { atoi(g[4].c_str()), atoi(g[5].c_str()), atoi(g[6].c_str()) };
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
            strcpy(dst, "Nu");
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

// [CAU C]: NHAP SO LUONG + VI TRI VA TU DONG DANH MA SACH
void SinhMaSach(DauSach &b) {
    int soLuong;
    string viTri;
    NhapSo(3, 28, "So luong sach: ", soLuong, 1, 1000, false);
    NhapChuoi(3, 29, "Vi tri ke: ", viTri, 29, false);
    ThemSachChoDauSach(&b, soLuong, viTri.c_str());
}

// [CAU D]: IN DAU SACH THEO TUNG THE LOAI (TEN TANG DAN)
void InDS_DauSach(DS_DauSach *ds) {
        const char* TITLE = "IN DAU SACH THEO TUNG THE LOAI (TEN TANG DAN)";
        system("cls");
        drawBanner(25, 2, TITLE);
        int y = 5;

        MangDong<string> theLoai;
        for (int i = 0; i < ds->n; ++i) {
            bool daCo = false;
            for (size_t j = 0; j < theLoai.size(); ++j)
                if (StrICmp(ds->ds[i]->TheLoai.c_str(), theLoai[j].c_str()) == 0) { daCo = true; break; }
            if (!daCo) theLoai.push_back(ds->ds[i]->TheLoai);
        }
        sort(theLoai.begin(), theLoai.end(),
             [](const string& a, const string& b) { return StrICmp(a.c_str(), b.c_str()) < 0; });

        for (size_t t = 0; t < theLoai.size(); ++t) {
            CheckPage(y, TITLE);
            setColor(14, 0);
            gotoxy(5, y++); cout << ">> THE LOAI: " << theLoai[t];
            setColor(15, 0);
            for (int i = 0; i < ds->n; ++i) {   // DS DA SAP XEP TANG DAN THEO TEN SACH
                if (StrICmp(ds->ds[i]->TheLoai.c_str(), theLoai[t].c_str()) != 0) continue;
                CheckPage(y, TITLE);
                gotoxy(7, y++);
                cout << "- [" << ds->ds[i]->ISBN << "] " << ds->ds[i]->TenSach
                     << " (" << ds->ds[i]->TacGia << ", " << ds->ds[i]->NamXuatBan << ")";
            }
            y++;
        }
        if (theLoai.empty()) { gotoxy(5, y++); setColor(12, 0); cout << "(!) Chua co dau sach nao!"; }
        gotoxy(5, y); setColor(10, 0); cout << "Nhan phim bat ky de quay lai...";
        _getch();
}

// [CAU E]: TIM THONG TIN SACH THEO TEN SACH
void TimSachTheoTen(DS_DauSach DS, PTR_DanhMucSach DMS, string TenSach) {
        const char* TITLE = "KET QUA TIM KIEM THONG TIN SACH THEO TEN";
        system("cls");
        drawBanner(20, 2, TITLE);
        int y = 5;
        bool found = false;
        for (int i = 0; i < DS.n; ++i) {
            if (!ContainsNoCase(DS.ds[i]->TenSach.c_str(), TenSach.c_str())) continue;
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
                    if (TimDauSachTheoISBN(&DS, isbn) != -1) {
                        showCursor(false);
                        thongBao("(!) Ma ISBN nay da ton tai trong he thong!", 12, 1300);
                        clearArea(3, 21, 95, 10);
                        break;
                    }

                    DauSach d = DauSach();
                    d.ISBN = isbn;
                    string s; int v;
                    NhapChuoi(3, 23, "Ten sach: ", s, 59, false);  d.TenSach = s;
                    NhapChuoi(3, 24, "Tac gia: ", s, 39, false);   d.TacGia = s;
                    NhapSo(3, 25, "So trang: ", v, 1, 100000, false);  d.SoTrang = v;
                    NhapSo(3, 26, "Nam xuat ban: ", v, 1000, LayNgayHienTai().Nam, false);  d.NamXuatBan = v;
                    NhapChuoi(3, 27, "The loai: ", s, 29, false);  d.TheLoai = s;
                    SinhMaSach(d);
                    ThemDauSach(&DS, d);

                    int pos = TimDauSachTheoISBN(&DS, d.ISBN);
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

                    TimSachTheoTen(DS, NULL, kw);
                    drawStatic();
                    break;
                }

                case KEY_F5: { // CAU D: IN DAU SACH THEO TUNG THE LOAI
                    InDS_DauSach(&DS);
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
int g_MaTheThaoTac = -1;   // ma the vua them / sua, de bang nhay toi dong do

// [CAU A]: THEM THE DOC GIA
void ThemTheDocGia(PTR_TheDocGia& root) {
    showCursor(true);
    clearArea(3, 21, 95, 10);
    setColor(14, 0);
    gotoxy(3, 21); cout << "--- THEM THE DOC GIA MOI ---";
    TheDocGia dg;
    dg.MaThe = SinhMaThe(root);
    if (dg.MaThe == -1) {
        showCursor(false);
        thongBao("(!) Da het kho ma the co the cap!", 12, 1200);
        return;
    }
    gotoxy(3, 22); cout << "Ma the (tu dong): " << dg.MaThe;
    string s;
    NhapChuoi(3, 23, "Ho: ", s, 39, false);  CopyStr(dg.ho, sizeof(dg.ho), s);
    NhapChuoi(3, 24, "Ten: ", s, 19, false); CopyStr(dg.ten, sizeof(dg.ten), s);
    NhapPhai(3, 25, "Phai (Nam/Nu): ", dg.phai, false);
    dg.TrangThaiThe = 1;
    dg.mt = nullptr;
    g_MaTheThaoTac = dg.MaThe;
    InsertDocGia(root, dg);

    showCursor(false);
    clearArea(3, 21, 95, 10);
    thongBao("=> Them doc gia thanh cong! (nho nhan F2 de luu)", 10, 1000);
}

// [CAU A]: HIEU CHINH THE DOC GIA
void HieuChinhDocGia(PTR_TheDocGia root) {
    showCursor(true);
    clearArea(3, 21, 95, 10);
    setColor(14, 0);
    gotoxy(3, 21); cout << "--- HIEU CHINH THE DOC GIA ---";
    int maThe;
    if (!NhapSo(3, 22, "Nhap ma the can chinh (Enter de huy): ", maThe, 1, 999999999, true)) {
        showCursor(false);
        clearArea(3, 21, 95, 10);
        return;
    }
    PTR_TheDocGia p = SearchDocGia(root, maThe);
    if (!p) {
        showCursor(false);
        clearArea(3, 21, 95, 10);
        thongBao("(!) Khong tim thay ma the!", 12, 1000);
        return;
    }
    TheDocGia* d = &(p->data);
    clearArea(3, 21, 95, 10);
    setColor(14, 0);
    gotoxy(3, 21); cout << "--- HIEU CHINH DOC GIA MA THE: " << d->MaThe << " (Enter = giu nguyen) ---";
    string s;
    NhapChuoi(3, 22, string("Ho [") + d->ho + "]: ", s, 39, true);
    if (!s.empty()) CopyStr(d->ho, sizeof(d->ho), s);
    NhapChuoi(3, 23, string("Ten [") + d->ten + "]: ", s, 19, true);
    if (!s.empty()) CopyStr(d->ten, sizeof(d->ten), s);
    NhapPhai(3, 24, string("Phai [") + d->phai + "] (Nam/Nu): ", d->phai, true);
    int tt;
    if (NhapSo(3, 25, "Trang thai the (0: Khoa, 1: Hoat dong) [" + to_string(d->TrangThaiThe) + "]: ", tt, 0, 1, true))
        d->TrangThaiThe = tt;

    g_MaTheThaoTac = d->MaThe;
    showCursor(false);
    clearArea(3, 21, 95, 10);
}

// [CAU A]: XOA THE DOC GIA
void XoaDocGia(PTR_TheDocGia &root) {
    showCursor(true);
    clearArea(3, 21, 95, 10);
    setColor(14, 0);
    gotoxy(3, 21); cout << "--- XOA THE DOC GIA ---";
    int maThe;
    if (!NhapSo(3, 22, "Nhap ma the can xoa (Enter de huy): ", maThe, 1, 999999999, true)) {
        showCursor(false);
        clearArea(3, 21, 95, 10);
        return;
    }
    showCursor(false);
    if (TimMaThe(root, maThe) == nullptr) {
        clearArea(3, 21, 95, 10);
        thongBao("(!) Khong tim thay ma the!", 12, 1000);
        return;
    }
    if (HoiYN(3, 23, "Ban co chac muon xoa the doc gia " + to_string(maThe) + "?")) {
        clearArea(3, 21, 95, 10);
        if (DelDocGia(root, maThe)) thongBao("=> Da xoa the doc gia thanh cong!", 10, 1000);
        else thongBao("(!) Doc gia dang muon sach, khong xoa duoc!", 12, 1300);
    }
    clearArea(3, 21, 95, 10);
}

// [CAU B]: IN DANH SACH DOC GIA THEO TEN + HO TANG DAN
void InDS_DocGia(PTR_TheDocGia root) {
    TheDocGia* arr[MAXDOCGIA];
    int count = 0;
    chuyenCayVaoMang(root, arr, count);
    stable_sort(arr, arr + count, [](const TheDocGia* a, const TheDocGia* b) {
        int c = StrICmp(a->ten, b->ten);
        if (c != 0) return c < 0;
        return StrICmp(a->ho, b->ho) < 0;
    });

    MangDong<Row> rows;
    for (int i = 0; i < count; ++i) {
        Row r;
        r.push_back(to_string(i + 1));
        r.push_back(to_string(arr[i]->MaThe));
        r.push_back(arr[i]->ho);
        r.push_back(arr[i]->ten);
        r.push_back(arr[i]->phai);
        r.push_back(arr[i]->TrangThaiThe == 1 ? "Hoat dong" : "Bi khoa");
        rows.push_back(r);
    }
    int colW[] = { 6, 15, 28, 15, 8, 18 };
    const char* const hd[] = { "STT", "MA THE", "HO", "TEN", "PHAI", "TRANG THAI" };
    viewBang("DANH SACH DOC GIA (TEN + HO TANG DAN)", "", colW, 6, hd, rows);
}

void viewTheDocGia(PTR_TheDocGia& root) {
    int colW[] = { 6, 15, 28, 15, 8, 18 };
    const int nc = 6;
    Row header;
    header.push_back("STT");  header.push_back("MA THE"); header.push_back("HO");
    header.push_back("TEN");  header.push_back("PHAI");   header.push_back("TRANG THAI");

    Hint hints[] = { {"ESC", "Exit"}, {"F2", "Save"}, {"INS", "Them"}, {"F3", "Sua"},
                     {"DEL", "Xoa"}, {"F5", "Xep Ten / Ma"}, {"F6", "In DS"} };

    auto drawStatic = [&]() {
        showCursor(false);
        system("cls");
        drawBanner(25, 2, "DANH SACH THE DOC GIA (CAY NHI PHAN TIM KIEM BST)");
        drawTableFrame(3, 4, colW, nc, LINES_PER_PAGE);
        drawRow(3, 5, colW, nc, header, 15, 0);
        drawNavigationGuide(104, 8);
        drawHints(19, hints, 7);
    };

    TheDocGia* arr[MAXDOCGIA];
    int tong = 0;
    bool sortByName = false;

    auto rebuild = [&]() {
        tong = 0;
        chuyenCayVaoMang(root, arr, tong);
        if (sortByName) {
            stable_sort(arr, arr + tong, [](const TheDocGia* a, const TheDocGia* b) {
                int c = StrICmp(a->ten, b->ten);
                if (c != 0) return c < 0;
                return StrICmp(a->ho, b->ho) < 0;
            });
        }
    };

    auto getRow = [&](int idx, Row& c) {
        c[0] = to_string(idx + 1);
        c[1] = to_string(arr[idx]->MaThe);
        c[2] = arr[idx]->ho;
        c[3] = arr[idx]->ten;
        c[4] = arr[idx]->phai;
        c[5] = (arr[idx]->TrangThaiThe == 1) ? "Hoat dong" : "Bi khoa";
    };

    int curPage = 1, curRow = 0;
    auto nhayToiMaThe = [&](int ma) {
        for (int i = 0; i < tong; ++i)
            if (arr[i]->MaThe == ma) { curPage = i / LINES_PER_PAGE + 1; curRow = i % LINES_PER_PAGE; break; }
    };

    drawStatic();
    rebuild();

    while (true) {
        int total = tong;
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

                case KEY_INSERT: // [CAU A]: THEM THE DOC GIA
                    g_MaTheThaoTac = -1;
                    ThemTheDocGia(root);
                    rebuild();
                    nhayToiMaThe(g_MaTheThaoTac);
                    break;

                case KEY_DELETE: // [CAU A]: XOA THE DOC GIA
                    XoaDocGia(root);
                    rebuild();
                    break;

                case KEY_F3: // [CAU A]: HIEU CHINH THE DOC GIA
                    g_MaTheThaoTac = -1;
                    HieuChinhDocGia(root);
                    rebuild();   // Ten/Ho co the doi -> sap xep lai
                    nhayToiMaThe(g_MaTheThaoTac);
                    break;

                case KEY_F2:
                    if (SaveTheDocGia(root)) thongBao("Da luu vao file THEDOCGIA.TXT thanh cong!", 10, 900);
                    else thongBao("(!) Khong ghi duoc file THEDOCGIA.TXT!", 12, 1200);
                    break;

                case KEY_F5: // CAU B: CHUYEN THE DOC GIA XEP THEO TEN+HO VA XEP MA DOC GIA
                    sortByName = !sortByName;
                    rebuild();
                    curPage = 1; curRow = 0;
                    break;

                case KEY_F6: // CAU B: IN DANH SACH DOC GIA
                    InDS_DocGia(root);
                    drawStatic();
                    break;
            }
        } else if (ch == KEY_ESC) {
            return;
        }
    }
}

// ===================== MAN HINH 3: MUON TRA SACH (CAU F, G, H) =====================
// IN LICH SU MUON TRA CUA 1 DOC GIA (dong thoi dem so sach dang muon)
void InDS_MuonTra(PTR_TheDocGia &p, int &BookSum) {
    MangDong<Row> rows;
    int stt = 1;
    for (PTR_MuonTra q = p->data.mt; q != NULL; q = q->next) {
        Row r;
        r.push_back(to_string(stt++));
        r.push_back(q->data.MaSach);
        r.push_back(DateStr(q->data.NgayMuon));
        r.push_back(q->data.TrangThai == 0 ? string("Chua tra") : DateStr(q->data.NgayTra));
        if (q->data.TrangThai == 0)      { r.push_back("Dang muon"); BookSum++; }
        else if (q->data.TrangThai == 1) r.push_back("Da tra");
        else                             r.push_back("Lam mat sach");
        rows.push_back(r);
    }
    int colW[] = { 6, 20, 15, 15, 18 };
    const char* const hd[] = { "STT", "MA SACH", "NGAY MUON", "NGAY TRA", "TRANG THAI" };
    string sub = string("Ma the: ") + to_string(p->data.MaThe) + " | Doc gia: " + p->data.ho + " " + p->data.ten;
    viewBang("LICH SU MUON TRA CUA DOC GIA", sub, colW, 5, hd, rows);
}

// [CAU H]: VE BANG CAC SACH DANG MUON CUA 1 DOC GIA
void InDS_Muon(PTR_TheDocGia p, DS_DauSach DS) {
    NgayThang today = LayNgayHienTai();
    MangDong<PTR_MuonTra> loans;
    for (PTR_MuonTra q = p->data.mt; q != nullptr; q = q->next)
        if (q->data.TrangThai == 0) loans.push_back(q);
    int cntMuon = (int)loans.size();

    int colMT[] = { 6, 18, 35, 15, 15 };
    const int nc = 5;
    const int rowsMT = MAX_MUON_SACH + 2;
    Row header;
    header.push_back("STT"); header.push_back("MA SACH"); header.push_back("TEN SACH");
    header.push_back("NGAY MUON"); header.push_back("TINH TRANG");

    clearArea(0, 4, 118, 20);
    setColor(11, 0);
    gotoxy(5, 4);
    cout << "Ma the: " << p->data.MaThe << " | Doc gia: " << p->data.ho << " " << p->data.ten
         << " | Phai: " << p->data.phai
         << " | Trang thai: " << (p->data.TrangThaiThe == 1 ? "Hoat dong" : "BI KHOA");

    drawTableFrame(5, 6, colMT, nc, rowsMT);
    drawRow(5, 7, colMT, nc, header, 15, 0);
    drawBody(5, 9, colMT, nc, cntMuon, 0, rowsMT, -1, [&](int idx, Row& c) {
        int days = SoNgay(loans[idx]->data.NgayMuon, today);
        c[0] = to_string(idx + 1);
        c[1] = loans[idx]->data.MaSach;
        c[2] = TimTenSach(DS, loans[idx]->data.MaSach);
        c[3] = DateStr(loans[idx]->data.NgayMuon);
        c[4] = (days > HAN_MUON_NGAY) ? "QUA HAN!" : "Binh thuong";
    });
    setColor(15, 0);
}

// [CAU H]: LIET KE SACH DANG MUON THEO MA THE
void LietKeSachMuon(int Mathe, PTR_TheDocGia &root, DS_DauSach DS) {
    PTR_TheDocGia p = SearchDocGia(root, Mathe);
    if (p == NULL) {
        thongBao("(!) KHONG TIM THAY MA THE!", 12, 1200);
        return;
    }
    system("cls");
    drawBanner(25, 2, "SACH DANG MUON CUA DOC GIA");
    InDS_Muon(p, DS);
    gotoxy(5, 17); setColor(10, 0);
    cout << "Nhan phim bat ky de quay lai...";
    setColor(15, 0);
    _getch();
}

// (them) NHAP MA THE VA TIM DOC GIA, tra ve nullptr neu huy / khong thay
PTR_TheDocGia NhapMaTheDocGia(PTR_TheDocGia &root) {
    showCursor(true);
    int maThe;
    if (!NhapSo(5, 5, "NHAP MA THE DOC GIA (Enter de quay lai): ", maThe, 1, 999999999, true)) {
        showCursor(false);
        return nullptr;
    }
    showCursor(false);
    PTR_TheDocGia p = TimMaThe(root, maThe);
    if (!p) thongBao("(!) KHONG TIM THAY MA THE!", 12, 1200);
    return p;
}

// [CAU F]: MUON SACH
void MuonSach(PTR_TheDocGia &root, DS_DauSach &DS) {
    system("cls");
    drawBanner(25, 2, "MUON SACH");
    PTR_TheDocGia p = NhapMaTheDocGia(root);
    if (p == nullptr) return;

    while (true) {
        InDS_Muon(p, DS);
        int cntMuon = 0;
        for (PTR_MuonTra q = p->data.mt; q != nullptr; q = q->next)
            if (q->data.TrangThai == 0) cntMuon++;

        if (p->data.TrangThaiThe == 0) { thongBao("(!) The bi khoa, khong duoc phep muon!", 12, 1500); return; }
        if (coQuaHan(p) == 1) { thongBao("(!) Doc gia giu sach qua 7 ngay, bi chan muon!", 12, 1500); return; }
        if (cntMuon >= MAX_MUON_SACH) { thongBao("(!) Da muon toi da 3 cuon sach!", 12, 1500); return; }

        showCursor(true);
        string maSach;
        NhapChuoi(5, 18, "Nhap Ma Sach muon muon (Enter de huy): ", maSach, 29, true);
        showCursor(false);
        if (maSach.empty()) return;
        ToUpper(maSach);

        PTR_DanhMucSach sach = TimSach(DS, maSach);
        if (!sach) { thongBao("(!) Ma sach khong ton tai!", 12, 1200); continue; }
        if (sach->data.TrangThai != 0) { thongBao("(!) Sach khong co san de cho muon!", 12, 1200); continue; }

        DauSach* dauSach = TimDauSachChuaMaSach(DS, maSach);
        sach->data.TrangThai = 1;
        if (dauSach) dauSach->SoLuotMuon++;
        PTR_MuonTra nMT = new DS_MuonTra;
        CapNhatSachMuon(nMT, sach->data.MaSach);
        nMT->next = p->data.mt;
        p->data.mt = nMT;

        thongBao("=> CHO MUON THANH CONG!", 10, 1200);
    }
}

// [CAU G]: TRA SACH
void TraSach(PTR_TheDocGia &root, DS_DauSach &DS) {
    system("cls");
    drawBanner(25, 2, "TRA SACH");
    PTR_TheDocGia p = NhapMaTheDocGia(root);
    if (p == nullptr) return;

    while (true) {
        InDS_Muon(p, DS);
        bool coMuon = false;
        for (PTR_MuonTra q = p->data.mt; q != nullptr; q = q->next)
            if (q->data.TrangThai == 0) { coMuon = true; break; }
        if (!coMuon) { thongBao("(!) Doc gia khong giu cuon sach nao!", 12, 1200); return; }

        showCursor(true);
        string maSach;
        NhapChuoi(5, 18, "Nhap Ma Sach can tra (Enter de huy): ", maSach, 29, true);
        showCursor(false);
        if (maSach.empty()) return;
        ToUpper(maSach);

        PTR_MuonTra curMT = TimSachDaMuon(p->data.mt, maSach);
        if (curMT == nullptr || curMT->data.TrangThai != 0) {
            thongBao("(!) Doc gia khong dang muon cuon sach nay!", 12, 1200);
            continue;
        }
        PTR_DanhMucSach sach = TimSach(DS, curMT->data.MaSach);

        gotoxy(5, 19); setColor(14, 0);
        cout << "Hinh thuc: [1] Tra binh thuong   [2] Lam mat sach   [ESC] Huy: ";
        setColor(15, 0);
        int ht = _getch();
        if (ht != '1' && ht != '2') continue;

        curMT->data.NgayTra = LayNgayHienTai();
        if (ht == '2') {
            curMT->data.TrangThai = 2;               // LAM MAT
            if (sach) sach->data.TrangThai = 2;      // THANH LY
        } else {
            curMT->data.TrangThai = 1;               // DA TRA
            if (sach) sach->data.TrangThai = 0;      // TRA VE KHO
        }
        thongBao("=> GHI NHAN TRA SACH THANH CONG!", 10, 1200);
    }
}

void viewMuonTra(PTR_TheDocGia &root, DS_DauSach &DS) {
    while (true) {
        showCursor(false);
        system("cls");
        drawBanner(25, 2, "NGHIEP VU MUON - TRA SACH");
        setColor(14, 0);
        gotoxy(5, 5);  cout << "[1] Muon sach";
        gotoxy(5, 7);  cout << "[2] Tra sach";
        gotoxy(5, 9);  cout << "[3] Liet ke sach dang muon cua doc gia";
        gotoxy(5, 11); cout << "[4] Lich su muon tra cua doc gia";
        gotoxy(5, 13); cout << "[ESC] Quay lai";
        setColor(15, 0);

        int opt = _getch();
        if (opt == KEY_ESC) return;
        if (opt == '1') MuonSach(root, DS);
        else if (opt == '2') TraSach(root, DS);
        else if (opt == '3' || opt == '4') {
            system("cls");
            drawBanner(25, 2, "NGHIEP VU MUON - TRA SACH");
            showCursor(true);
            int maThe;
            if (!NhapSo(5, 5, "NHAP MA THE DOC GIA (Enter de quay lai): ", maThe, 1, 999999999, true)) continue;
            showCursor(false);
            if (opt == '3') {
                LietKeSachMuon(maThe, root, DS);
            } else {
                PTR_TheDocGia p = SearchDocGia(root, maThe);
                if (!p) thongBao("(!) KHONG TIM THAY MA THE!", 12, 1200);
                else { int BookSum = 0; InDS_MuonTra(p, BookSum); }
            }
        }
    }
}

// ===================== MAN HINH 4: THONG KE QUA HAN & TOP 10(CAU I,J) =====================
struct DGQuaHan {
    PTR_TheDocGia docgia;
    int TongNgayQuaHan;
};

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
                r.HoTen = string(root->data.ho) + " " + root->data.ten;
                r.MaSach = p->data.MaSach;
                r.NgayMuon = p->data.NgayMuon;
                r.SoNgayTre = days - HAN_MUON_NGAY;
                arr.push_back(r);
            }
        }
    }

    DuyetQuaHan(root->right, arr, today);
}

// TONG SO NGAY QUA HAN CUA CAC SACH DANG MUON
int ThoiGianMuon(PTR_MuonTra &mt) {
    NgayThang NgayHienTai = LayNgayHienTai();
    int SoNgayQuaHan = 0;
    for (PTR_MuonTra p = mt; p != NULL; p = p->next) {
        if (p->data.TrangThai == 0) {
            int SoNgayMuon = SoNgay(p->data.NgayMuon, NgayHienTai);
            if (SoNgayMuon > HAN_MUON_NGAY) SoNgayQuaHan += (SoNgayMuon - HAN_MUON_NGAY);
        }
    }
    return SoNgayQuaHan;
}

void TGQuaHanGiamDan(DGQuaHan DG[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int largest = i;
        for (int j = i + 1; j < n; j++) {
            if (DG[j].TongNgayQuaHan > DG[largest].TongNgayQuaHan) largest = j;
        }
        DGQuaHan tmp = DG[i];
        DG[i] = DG[largest];
        DG[largest] = tmp;
    }
}

void NLR_TheDocGia(PTR_TheDocGia root, DGQuaHan DG[], int &pos) {
    if (root == NULL) return;
    int TongNgayQuaHan = ThoiGianMuon(root->data.mt);
    if (TongNgayQuaHan > 0) {
        DG[pos].docgia = root;
        DG[pos].TongNgayQuaHan = TongNgayQuaHan;
        pos++;
    }
    NLR_TheDocGia(root->left, DG, pos);
    NLR_TheDocGia(root->right, DG, pos);
}

// [CAU I]: DANH SACH DOC GIA MUON SACH QUA HAN (GIAM DAN)
void INDS_QuaHan(PTR_TheDocGia &root) {
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

void NLR_DemLuotMuon(PTR_TheDocGia root, string MaSach, int &cnt) {
    if (root == NULL) return;
    for (PTR_MuonTra p = root->data.mt; p != NULL; p = p->next) {
        if (p->data.MaSach == MaSach) cnt++;
    }
    NLR_DemLuotMuon(root->left, MaSach, cnt);
    NLR_DemLuotMuon(root->right, MaSach, cnt);
}

struct DS_Muon {
    DauSach *ds;
    int TongLanMuon = 0;
};

int DemTongLanMuon(DauSach* ds, PTR_TheDocGia root) {
    int TongCnt = 0;
    for (PTR_DanhMucSach p = ds->dms; p != NULL; p = p->next) {
        int Cnt = 0;
        NLR_DemLuotMuon(root, p->data.MaSach, Cnt);
        TongCnt += Cnt;
    }
    return TongCnt;
}

// [CAU J]: TOP SACH CO LUOT MUON NHIEU NHAT (xep theo SoLuotMuon cua dau sach)
void TopSach(DS_DauSach *DS, int soluong, PTR_TheDocGia root) {
    (void)root;
    MangDong<DS_Muon> DSM;
    for (int i = 0; i < DS->n; i++) {
        DS_Muon m;
        m.ds = DS->ds[i];
        m.TongLanMuon = DS->ds[i]->SoLuotMuon;
        DSM.push_back(m);
    }
    stable_sort(DSM.begin(), DSM.end(), [](const DS_Muon& a, const DS_Muon& b) {
        return a.TongLanMuon > b.TongLanMuon;
    });

    MangDong<Row> rows;
    int top = min((int)DSM.size(), soluong);
    for (int i = 0; i < top; ++i) {
        Row r;
        r.push_back(to_string(i + 1));
        r.push_back(DSM[i].ds->ISBN);
        r.push_back(DSM[i].ds->TenSach);
        r.push_back(DSM[i].ds->TheLoai);
        r.push_back(to_string(DSM[i].TongLanMuon));
        rows.push_back(r);
    }
    int colW[] = { 6, 15, 35, 18, 15 };
    const char* const hd[] = { "TOP", "ISBN", "TEN SACH", "THE LOAI", "LUOT MUON" };
    string title = "TOP " + to_string(soluong) + " SACH CO LUOT MUON NHIEU NHAT";
    viewBang(title.c_str(), "", colW, 5, hd, rows);
}

// ===================== DU LIEU KHOI TAO MAU =====================
void KhoiTaoDuLieuMau(DS_DauSach& DS, PTR_TheDocGia& root) {
    DauSach ds1 = DauSach();
    ds1.ISBN = "INT1358";
    ds1.TenSach = "Cau Truc Du Lieu & Giai Thuat";
    ds1.SoTrang = 380; ds1.TacGia = "Nguyen Van A";
    ds1.NamXuatBan = 2026; ds1.TheLoai = "Giao trinh";
    ds1.SoLuotMuon = 15;
    ThemSachChoDauSach(&ds1, 4, "Ke A1-01");
    ThemDauSach(&DS, ds1);

    DauSach ds2 = DauSach();
    ds2.ISBN = "INT1306";
    ds2.TenSach = "Lap Trinh Huong Doi Tuong";
    ds2.SoTrang = 420; ds2.TacGia = "Tran Van B";
    ds2.NamXuatBan = 2026; ds2.TheLoai = "Giao trinh";
    ds2.SoLuotMuon = 22;
    ThemSachChoDauSach(&ds2, 3, "Ke A1-02");
    ThemDauSach(&DS, ds2);

    DauSach ds3 = DauSach();
    ds3.ISBN = "KN001";
    ds3.TenSach = "Dac Nhan Tam";
    ds3.SoTrang = 320; ds3.TacGia = "Dale Carnegie";
    ds3.NamXuatBan = 2024; ds3.TheLoai = "Ky nang";
    ds3.SoLuotMuon = 8;
    ThemSachChoDauSach(&ds3, 2, "Ke B2-05");
    ThemDauSach(&DS, ds3);

    TheDocGia dg1(10431, "Nguyen Van", "An", "Nam", 1, nullptr);
    InsertDocGia(root, dg1);

    TheDocGia dg2(10442, "Le Thi", "Binh", "Nu", 1, nullptr);
    InsertDocGia(root, dg2);
}

// ===================== MENU CHINH THANH SANG HIGHLIGHT =====================
int main() {
    system("mode con: cols=120 lines=35");
    srand(static_cast<unsigned int>(time(nullptr)));

    DS_DauSach DS;
    PTR_TheDocGia root = NULL;
    PTR_DanhMucSach DMS = NULL;
    PTR_MuonTra MT = NULL;
    (void)DMS; (void)MT;

    ReadDauSach(&DS);
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
                case 3: INDS_QuaHan(root); veLaiMenuChinh(); break;
                case 4: TopSach(&DS, 10, root); veLaiMenuChinh(); break;
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