#include <bits/stdc++.h>
using namespace std;
const long long MAXSACH = 10000;

struct ThongTinSach{ //Node cho DanhMucSach
	string MaSach;
	int TrangThai;
	string ViTri;
};
struct DanhMucSach{
	ThongTinSach data;
	DanhMucSach* next;
};
typedef DanhMucSach *PTR_DanhMucSach;

struct DauSach{
	string ISBN, TenSach, TacGia, TheLoai;
	int SoTrang, NamXuatBan;
	PTR_DanhMucSach dms = NULL;
};
struct DS_DauSach{
	DauSach* ds[MAXSACH];
	int n = 0;
};

struct NgayThang{
	int Ngay;
	int Thang;
	int Nam;
};
struct MuonTra{ //Node cho DS_MUONTRA
	string MaSach;
	NgayThang NgayMuon, NgayTra;
	int TrangThai;
};

struct DS_MuonTra{
	MuonTra data;
	DS_MuonTra *next;
};
typedef DS_MuonTra *PTR_MuonTra;

struct TheDocGia{
	int MaThe;
	string Ho, Ten, Phai, TrangThai;
	PTR_MuonTra mt = NULL;
};
struct DS_TheDocGia{
	TheDocGia data;
	DS_TheDocGia *left, *right;
};
typedef DS_TheDocGia *PTR_TheDocGia;

//Cau A
int SinhMaThe(TheDocGia &TDG){}
PTR_TheDocGia TimMaThe(PTR_TheDocGia &root, int MaThe){}
void ThemDocGia(PTR_TheDocGia &p, TheDocGia TDG){}
void XoaDocGia(PTR_TheDocGia &p, int MaThe){}

//Cau B
void SortByName(PTR_TheDocGia root){}
//Tao cay moi voi key = ten//
void InDS_DocGia(PTR_TheDocGia root){}

//Cau C
void SinhMaSach(DauSach &b);
void ThemDauSach(DS_DauSach *ds, DauSach b){};

//Cau D
void InDS_DauSach(DS_DauSach *ds){}//In theo thể loại, trong thể loại lại in theo tên tăng dần

//Cau E
void TimSachTheoTen(PTR_DanhMucSach DMS, string TenSach){}

//Cau F
void MuonSach(PTR_TheDocGia &p,PTR_DanhMucSach &DMS, int MaThe){}

//Cau G
void TraSach(PTR_TheDocGia &root, PTR_DanhMucSach &DMS,int Mathe, int MaSach){}

//Cau H
void LietKeSachMuon(int Mathe, PTR_TheDocGia &p){}

//Cau I
int ThoiGianMuon(int NgayMuon){};
void INDS_QuaHan(PTR_TheDocGia &root){};

//Cau J
void Top10Sach(DauSach *ds, PTR_MuonTra MT){}
int main(){
	PTR_TheDocGia root = NULL;
	PTR_DanhMucSach DMS = NULL;
	PTR_MuonTra MT = NULL;
}
