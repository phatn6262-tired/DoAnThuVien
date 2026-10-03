#include <bits/stdc++.h>
#include <mylib.h>
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
PTR_DanhMucSach TimSach(DS_DauSach DS, string MaSach){
	for (int i =0; i<DS.n; i++){
		DauSach *p = DS.ds[i];
		PTR_DanhMucSach q;
		for (q = p->dms; q != NULL; q = q->next){
			if(MaSach == q->data.MaSach){
				return q;
			}
		}
	}
	return NULL;
}

//Thao tác Ngày Tháng
struct NgayThang{
	int Ngay;
	int Thang;
	int Nam;
};
void InNgayThang(NgayThang nt){
	cout << nt.Ngay<<"/"<<nt.Thang<<"/"<<nt.Nam;
}
int NamNhuan(int Nam){
	if(Nam % 400 == 0 || (Nam % 4 == 0 && Nam % 100 != 0)){
		return 1;
	}
	return 0;
}
int SoNgayTrongThang(int Thang, int Nam){
	if(Thang == 4 || Thang == 6 || Thang == 9 || Thang == 11)
		return 30;
	else if(Thang == 2){
		if(NamNhuan(Nam)) return 29;
		else return 28;
	}
	else return 31;
}
long long NgayToSoNgay(NgayThang d) {
    long long tong = 0;

    for (int y = 1; y < d.Nam; y++) {
        tong += NamNhuan(y) ? 366 : 365;
    }

    for (int m = 1; m < d.Thang; m++) {
        tong += SoNgayTrongThang(m, d.Nam);
    }

    tong += d.Ngay;

    return tong;
}
int SoNgay(NgayThang NgayMuon, NgayThang NgayHienTai){
	return NgayToSoNgay(NgayHienTai) - NgayToSoNgay(NgayMuon);
}
NgayThang LayNgayHienTai() {
    time_t now = time(0);
    tm* ltm = localtime(&now);

    NgayThang d;
    d.Ngay = ltm->tm_mday;
    d.Thang = ltm->tm_mon + 1;
    d.Nam = ltm->tm_year + 1900;

    return d;
}

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

void CapNhatSachMuon(PTR_MuonTra &n, string MaSach){
	n->data.MaSach = MaSach;
	n->data.NgayMuon = LayNgayHienTai();
	n->data.NgayTra = {0, 0, 0};
	n->data.TrangThai = 0;
	n->next = NULL;
}

struct TheDocGia{
	int MaThe;
	string Ho, Ten, Phai; 
	int TrangThai;
	PTR_MuonTra mt = NULL;
};
struct DS_TheDocGia{
	TheDocGia data;
	DS_TheDocGia *left, *right;
};
typedef DS_TheDocGia *PTR_TheDocGia;

void InDS_MuonTra(PTR_TheDocGia &p, int &BookSum){
	cout << left << setw(15) <<"Mã sách"<< setw(15) <<"Ngày mượn" << setw(15) <<"Ngày trả" << setw(15) <<"Trạng thái" << endl;
	PTR_MuonTra q;
	for (q = p->data.mt; q != NULL; q = q->next){
		cout << left << setw(15) << q->data.MaSach << setw(15);

		if(q->data.TrangThai == 0){
			InNgayThang(q->data.NgayMuon);
			cout << left << setw(15) << "Chưa trả" << setw(15) << "Đang mượn" << endl;
			BookSum++;
		}

		else if (q->data.TrangThai == 1){
			InNgayThang(q->data.NgayMuon);
			cout << left << setw(15); 
			InNgayThang(q->data.NgayTra); 
			cout << left << setw(15) << "Đã trả" << endl;
		}
		else{
			InNgayThang(q->data.NgayMuon);
			cout <<left << setw(15) << "Chưa trả" << setw(15) << "Làm mất sách" << endl;
		}
	}
}
PTR_TheDocGia TimMaThe(PTR_TheDocGia &root, int MaThe){
	if(root == NULL) return NULL;
	
	if (MaThe == root->data.MaThe) return root;

	if(MaThe < root->data.MaThe) return TimMaThe(root->left, MaThe);
	else return TimMaThe(root->right, MaThe);
}
int coQuaHan(PTR_TheDocGia &p){
	PTR_MuonTra q;
	NgayThang NgayHienTai = LayNgayHienTai();
	for (q = p->data.mt; q != NULL; q=q->next){
		if(q->data.TrangThai == 0){
			int tong = SoNgay(q->data.NgayMuon, NgayHienTai);
			if(tong > 7) return 1;
		}
	}
	return 0;
}

void MuonSach(PTR_TheDocGia &root, DS_DauSach &DS){
	PTR_TheDocGia p; int MaThe;

	do{
		cout << "Nhập mã thẻ độc gia: "; cin >> MaThe; cin.ignore();
		p = TimMaThe(root, MaThe);
		if(p == NULL)
			cout << "Mã thẻ không hợp lệ, nhập lại!" << endl;
		else{
			if(p->data.TrangThai == 0){
				cout <<"Thẻ đã bị khóa!" << endl;
				_getch();
				return;
			}
			int BookSum = 0;
			if(p->data.mt == NULL) cout <<"Độc giả không mược cuốn sách nào!" << endl;
			else InDS_MuonTra(p, BookSum);
			int ch;
			do{
				if(BookSum >= 3){
					cout << "Số sách đã mượn đạt giới hạn (3 cuốn)!";
					_getch(); return;
				}
				else if(coQuaHan(p) == 1){
					cout << "Độc giả đang mượn sách quá hạn!";
					_getch();
					return;
				}
				else{
					
					string MaSach;
					PTR_DanhMucSach s;
					
					do{
						cout << "Nhập mã sách bạn muốn mượn: ";
						getline(cin, MaSach);
						s = TimSach(DS,MaSach);
						if (s == NULL) {
							cout << "Mã sách không tồn tại!" << endl;
						}
					}while(s == NULL);
					
					if (s->data.TrangThai != 0) {
						if (s->data.TrangThai == 1)
							cout << "Sách đang được mượn!" << endl;
						else
							cout << "Sách đã thanh lý!" << endl;
							_getch();
							return;
					}
					PTR_MuonTra n = new DS_MuonTra;
					CapNhatSachMuon(n,MaSach);
					if (p->data.mt == NULL)
						p->data.mt = n;
					else{
					PTR_MuonTra q = p->data.mt;
					while(q->next != NULL)
						q=q->next;
					q->next = n;
					}

					s->data.TrangThai = 1;
					cout << "Mượn sách thành công!" << endl;
					BookSum++;
				}
				if(BookSum >= 3){
					cout << "Số sách đang mượn đã đủ ba cuốn!" << endl;
					_getch();
					return;
				}
				do {
   					cout << "Bạn có muốn tiếp tục mượn không? (1/0): ";
    				cin >> ch;

    				if (ch != 0 && ch != 1)
        				cout << "Chỉ được nhập 1 hoặc 0!\n";

				} while (ch != 0 && ch != 1);

				if(ch == 0) return;
			} while (ch == 1);
		} 
	} while(p == NULL);
}

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
}
