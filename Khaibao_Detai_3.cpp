#include <bits/stdc++.h>
#include "mylib.h"
using namespace std;
const long long MAXSACH = 10000;
const int MAXDOCGIA = 10000;

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
PTR_MuonTra TimSachDaMuon(PTR_MuonTra &p, string MaSach){
	PTR_MuonTra q;
	PTR_MuonTra pre_q = NULL;
	for (q = p; q != NULL; q = q->next){
		if(MaSach == q->data.MaSach){
			pre_q = q;
			if(q->data.TrangThai == 0){
				return q;
			}
		}
	}
	return pre_q;
}

struct TheDocGia{
	int MaThe;
	char Ho[40];
	char Ten[20];
	char Phai[5]; 
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
int MIN(int a, int b){
	if(a < b) return a;
	else return b;
}

//Câu F: Mượn sách
void MuonSach(PTR_TheDocGia &root, DS_DauSach &DS){
	PTR_TheDocGia p; int MaThe;

	do{
		cout << "Nhập mã thẻ độc giả: "; cin >> MaThe; cin.ignore();
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

//Câu G: Trả sách
void TraSach(PTR_TheDocGia &root, DS_DauSach &DS){
	PTR_TheDocGia p; int MaThe;
	
	do{
		cout <<"Nhập mã thẻ độc giả: ";
		cin >> MaThe;
		cin.ignore();
		p = TimMaThe(root, MaThe);
		if(p == NULL)
			cout << "Mã thẻ không hợp lệ, nhập lại!" << endl;
		else{
			int BookSum = 0;
			InDS_MuonTra(p,BookSum);
			if(BookSum == 0){
				cout << "Độc giả không mượn cuốn sách nào!";
				_getch();
				return;
			}
			PTR_MuonTra q;
			PTR_DanhMucSach exist;
			do{
				string Masach;
				cout << "Nhập mã thẻ của sách muốn trả: ";
				getline(cin, Masach);

				exist = TimSach(DS,Masach);
				if(exist == NULL){
						cout << "Mã sách không tồn tại, nhập lại!";
						continue;
				}

				q = TimSachDaMuon(p->data.mt,Masach);
				if(q == NULL){
					cout <<"Độc giả chưa từng mượn sách này!";
					continue;
				}
				else if(q->data.TrangThai == 1){
					cout << "Độc giả đã trả sách!";
					continue;
				}
				else if(q->data.TrangThai == 2){
					cout <<"Độc giả đã làm mất sách!";
					continue;
				}
				else{
					char xn;
					cout << "Xác nhận trả sách (Y/N) ?:";
					cin >> xn; cin.ignore();
					xn = tolower(xn);
					if(xn == 'y'){
						q->data.NgayTra = LayNgayHienTai();
						q->data.TrangThai = 1;
						exist->data.TrangThai = 0;
						cout << "Trả sách thành công!";
						return;
					}
					else{
						cout << "Đã hủy trả sách!";
						return;
					}
				}


			}while(exist == NULL || q == NULL ||q->data.TrangThai == 1|| q->data.TrangThai == 2);
		}
	} while (p == NULL);
}

//Câu H: Liệt kê sách mượn
string TimTenSach(const DS_DauSach &DS, string MaSach) {
    for (int i = 0; i < DS.n; i++) {
        DauSach *p = DS.ds[i];

        for (PTR_DanhMucSach q = p->dms; q != NULL;q = q->next) {
            if (q->data.MaSach == MaSach) {
                return p->TenSach;
            }
        }
    }

    return "Khong tim thay";
}
void InDS_Muon(PTR_TheDocGia p, DS_DauSach DS){
	cout << left << setw(15) <<"Mã sách"<< setw(30) << "Tên sách" << setw(15) <<"Ngày mượn" << setw(15) <<"Ngày trả" << setw(15) <<"Trạng thái" << endl;
	PTR_MuonTra q;
	bool coSachMuon = false;
	for (q = p->data.mt; q != NULL; q = q->next){
		if (q->data.TrangThai == 0){
            coSachMuon = true;

            string tenSach = TimTenSach(DS, q->data.MaSach);

            cout << left
                 << setw(15) << q->data.MaSach
                 << setw(30) << tenSach
                 << setw(15) << (
                     to_string(q->data.NgayMuon.Ngay) + "/" +
                     to_string(q->data.NgayMuon.Thang) + "/" +
                     to_string(q->data.NgayMuon.Nam)
                 )
                 << setw(15) << "Đang mượn"
                 << endl;
        }
	}
	if(coSachMuon == false){
		cout << "Độc giả hiện không có sách đang mượn!" << endl;
	}
}
void LietKeSachMuon(int Mathe, PTR_TheDocGia &root, DS_DauSach DS){
	PTR_TheDocGia p = TimMaThe(root, Mathe);
	if(p == NULL){
		cout << "Mã thẻ không tồn tại!";
		return;
	}
	InDS_Muon(p,DS);
}

//Câu I: Danh sách độc giả quá hạn
struct DGQuaHan {
    PTR_TheDocGia docgia;
    int TongNgayQuaHan;
};
int ThoiGianMuon(PTR_MuonTra &mt){
	NgayThang NgayHienTai = LayNgayHienTai();
	int SoNgayQuaHan = 0;
	PTR_MuonTra p;
	for (p = mt; p != NULL; p = p->next){
		if(p->data.TrangThai == 0){
			int SoNgayMuon = SoNgay(p->data.NgayMuon, NgayHienTai);
			if(SoNgayMuon > 7)
				SoNgayQuaHan += (SoNgayMuon-7);
			else
				continue;
		}
	}
	return SoNgayQuaHan;
}
void TGQuaHanGiamDan(DGQuaHan DG[], int n){
	for (int i = 0; i < n-1; i++){
		int largest = i;
		for (int j = i+1; j <n; j++){
			if(DG[j].TongNgayQuaHan > DG[largest].TongNgayQuaHan)
				largest = j;
		}
		DGQuaHan tmp = DG[i];
		DG[i] = DG[largest];
		DG[largest] = tmp;
	}
}
void NLR_TheDocGia(PTR_TheDocGia root, DGQuaHan DG[], int &pos){
	if(root == NULL) return;
	int TongNgayQuaHan = ThoiGianMuon(root->data.mt);
	if(TongNgayQuaHan > 0){
		DG[pos].docgia = root;
		DG[pos].TongNgayQuaHan = TongNgayQuaHan;
		pos++;
	}
	NLR_TheDocGia(root->left, DG, pos);
	NLR_TheDocGia(root->right, DG, pos);
}
void INDS_QuaHan(PTR_TheDocGia &root){
	DGQuaHan DG[MAXDOCGIA];
	int n = 0;
	NLR_TheDocGia(root, DG, n);
	if(n == 0){
		cout << "Không có độc giả nào mượn sách quá hạn!" << endl;
		return;
	}

	TGQuaHanGiamDan(DG, n);
	cout << "====================================DANH SÁCH ĐỘC GIẢ MƯỢN SÁCH QUÁ HẠN====================================" << endl;
	cout << endl;
	cout << left << setw(10) << "MÃ THẺ" << setw(40) << "HỌ" << setw(20) << "TÊN" << setw(5) << "PHÁI"  << setw(4) << "THỜI GIAN QUÁ HẠN" << endl;
	for (int i=0; i<n; i++){
		cout << left << setw(10) << DG[i].docgia->data.MaThe 
			 << setw(40) << DG[i].docgia->data.Ho
			 << setw(20) << DG[i].docgia->data.Ten
			 << setw(5) << DG[i].docgia->data.Phai
			 << setw(4) << DG[i].TongNgayQuaHan << endl;
	}
}

//Câu J: Top 10 sách được mượn
void NLR_DemLuotMuon(PTR_TheDocGia root, string MaSach, int &cnt){
	if (root == NULL) return;
	PTR_MuonTra p;
	for (p = root->data.mt; p != NULL; p = p->next){
		if(p->data.MaSach == MaSach) cnt++;
	}
	NLR_DemLuotMuon(root->left, MaSach, cnt);
	NLR_DemLuotMuon(root->right, MaSach, cnt);
}
struct DS_Muon{
	DauSach *ds;
	int TongLanMuon = 0;
};
int DemTongLanMuon(DauSach* ds, PTR_TheDocGia root){
	int TongCnt = 0;
	PTR_DanhMucSach p;
	for (p = ds->dms; p != NULL; p = p->next){
		int Cnt = 0;
		NLR_DemLuotMuon(root,p->data.MaSach, Cnt);
		TongCnt += Cnt;
	}
	return TongCnt;
}
void Top10Sach(DS_DauSach *DS, PTR_TheDocGia root){
	DS_Muon DSM[MAXSACH];
	for (int i = 0; i < DS->n; i++){
		DSM[i].ds = DS->ds[i];
		DSM[i].TongLanMuon = DemTongLanMuon(DS->ds[i],root);
	}
	for (int i = 0; i<DS->n-1; i++){
		int largest = i;
		for (int j = i+1; j<DS->n; j++){
			if(DSM[j].TongLanMuon > DSM[largest].TongLanMuon){
				largest = j;
			}
		}
		DS_Muon tmp = DSM[i];
		DSM[i] = DSM[largest];
		DSM[largest] = tmp;
	}
	int soLuongIn = MIN(10, DS->n);
	cout << "========================TOP "<< soLuongIn << "SÁCH ĐƯỢC MƯỢN NHIỀU NHẤT========================" << endl;
	cout << left << setw(5) << "STT" << setw(20) <<"MÃ ISBN" << setw(50) << "TÊN SÁCH" << setw(50) <<"TÁC GIẢ" << setw(15) << "THỂ LOẠI" << setw(15) << "SỐ LƯỢT MƯỢN" << endl;
	
	
	for (int i = 0; i<soLuongIn; i++){
		cout << left << setw(5) << i+1 
			 << setw(20) <<DSM[i].ds->ISBN
			 << setw(50) << DSM[i].ds->TenSach
			 << setw(50) << DSM[i].ds->TacGia
			 << setw(15) << DSM[i].ds->TheLoai
			 << setw(15) << DSM[i].TongLanMuon<< endl;
	}
}

int main(){
	PTR_TheDocGia root = NULL;
	PTR_DanhMucSach DMS = NULL;
}
