#include <bits/stdc++.h>
#include "mylib.h"
#include <conio.h>
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

//===========================MƯỢN TRẢ==============================
struct MuonTra{
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

//===================== THẺ ĐỘC GIẢ =====================
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
PTR_TheDocGia SearchDocGia(PTR_TheDocGia root, int MaThe) {
	if (root == nullptr || root->data.MaThe == MaThe) {
		return root;
	}
	if (MaThe < root->data.MaThe) {
		return SearchDocGia(root->left, MaThe);
	}
	return SearchDocGia(root->right, MaThe);
}
int SinhMaThe(PTR_TheDocGia root) {
	int ma;
	do {
		ma = rand() % 90000 + 10000;
	} while (SearchDocGia(root, ma) != nullptr);
	return ma;
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

void TimLop(DS_TheDocGia*& p, DS_TheDocGia*& q) {
	if (q->left != nullptr) {
		TimLop(p, q->left);
	} 
	else {
		p->data = q->data;
		DS_TheDocGia* temp = q;
		q = q->right;
		delete temp;
	}
}
bool InsertDocGia(PTR_TheDocGia &p, const TheDocGia &TDG) {
	if (p == nullptr) {
		p = new DS_TheDocGia;
		p->data = TDG;
		p->left = p->right = nullptr;
		return true;
	}
	if (TDG.MaThe < p->data.MaThe) {
		return InsertDocGia(p->left, TDG);
	}
	if (TDG.MaThe > p->data.MaThe) {
		return InsertDocGia(p->right, TDG);
	}
	return false;
}
bool DelDocGia(PTR_TheDocGia& p, int MaThe) {
	if (p == nullptr) return false;
	if (MaThe < p->data.MaThe) {
		return DelDocGia(p->left, MaThe);
	}
	if (MaThe > p->data.MaThe) {
		return DelDocGia(p->right, MaThe);
	}

	for (PTR_MuonTra cur = p->data.mt; cur != nullptr; cur = cur->next) {
        if (cur->data.TrangThai == 0) {
            cout << "Doc gia dang muon sach chua tra, 0 xoa the!\n";
            return false;
        }
	}
	DS_TheDocGia* temp = p;
	if (p->left == nullptr) p = p->right;
	else if (p->right == nullptr) p = p->left;
	else{
		TimLop(p, p->right);
		return true;
	}
	delete temp;
	return true;
	
}

// ===================== CÂU A: THẺ ĐỘC GIẢ =====================
void ThemTheDocGia(PTR_TheDocGia& root) {
	TheDocGia dg;
	dg.MaThe = SinhMaThe(root);
	cout << "\n them thanh cong \n";
	cout << " ma the " << dg.MaThe << "\n";
	cin.ignore();
	cout << "nhap ho: "; cin.getline(dg.Ho, 40);
	cout << "nhap ten: "; cin.getline(dg.Ten, 20);

	do {
		cout << " gioi tinh (Nam/Nu): ";
		cin.getline(dg.Phai, 5);
		if (stricmp(dg.Phai, "Nam") != 0 && stricmp(dg.Phai, "Nu") != 0) {
			cout << "chi duoc nhap Nam hoc Nu: \n";
		}
	} while (stricmp(dg.Phai, "Nam") != 0 && stricmp(dg.Phai, "Nu") != 0);

	dg.TrangThai = 1;
	dg.mt = nullptr;
	InsertDocGia(root, dg);
	cout << "them doc gia thanh cong";
}
void HieuChinhDocGia(PTR_TheDocGia root) {
	int mathe;
	cout << "\n nhap ma the can chinh: ";
	cin >> mathe;
	DS_TheDocGia* p = SearchDocGia(root, mathe);
	if (!p) {
		cout << "ko tim thay";
		return;
	}
	cin.ignore();
	cout << " nhap ho: ";
	cin.getline(p->data.Ho, 40);
	cout << " nhap ten: ";
	cin.getline(p->data.Ten, 20);

	do {
		cout << "nhap gioi tinh (Nam/Nu): ";
		cin.getline(p->data.Phai, 5);
    } while (stricmp(p->data.Phai, "Nam") != 0 && stricmp(p->data.Phai, "Nu") != 0);

    cout << "Trang thai the (0: Khoa, 1: Hoat dong): ";
    cin >> p->data.TrangThai;
    cout << "=> Cap nhat the thanh cong!\n";
}
void XoaDocGia(PTR_TheDocGia &root) {
	int maThe;
    cout << "\nNhap ma the can xoa: ";
    cin >> maThe;
    if (DelDocGia(root, maThe))
        cout << "=> Da xoa the doc gia thanh cong!\n";
    else
        cout << "(!) Xoa the that bai!\n";
}

//Cau B: In Danh Sach Doc Gia
void chuyenCayVaoMang(PTR_TheDocGia root, TheDocGia* arr[], int& count) {
    if (root == nullptr) return;
    chuyenCayVaoMang(root->left, arr, count);
    arr[count++] = &(root->data);
    chuyenCayVaoMang(root->right, arr, count);
}
void InDS_DocGia(PTR_TheDocGia root) {
    TheDocGia* arr[MAXSACH];
    int count = 0;
    chuyenCayVaoMang(root, arr, count);

    for (int i = 0; i < count - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < count; ++j) {
            int cmpTen = stricmp(arr[j]->Ten, arr[minIdx]->Ten);
            if (cmpTen < 0 || (cmpTen == 0 && stricmp(arr[j]->Ho	, arr[minIdx]->Ho) < 0)) {
                minIdx = j;
            }
        }
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
	cout << left << setw(10) << "MA THE" 
         << setw(25) << "HO" 
         << setw(15) << "TEN" 
         << setw(8) << "PHAI" 
         << setw(15) << "TRANG THAI" << "\n";
    cout << string(73, '-') << "\n";

    for (int i = 0; i < count; ++i) {
		cout << left << setw(10) << arr[i]->MaThe
	         << setw(25) << arr[i]->Ho
	         << setw(15) << arr[i]->Ten
	         << setw(8) << arr[i]->Phai
	         << setw(15) << (arr[i]->TrangThai == 1 ? "Hoat dong" : "Bi khoa") << "\n";
    }
}

//Cau C: Nhap thong tin sao cho tim thay sach nhanh nhat
int DemSoSachCon(PTR_DanhMucSach DMS) {
    int cnt = 0;
    for (PTR_DanhMucSach p = DMS; p != nullptr; p = p->next) cnt++;
    return cnt;
}
void SinhMaSach(DauSach &b){
	int soLuong;
		cout << "Nhap so luong ban in can them cho dau sach: ";
		cin >> soLuong;
		cin.ignore();
		
		string viTri;
		cout << "Nhap vi tri dat sach tren ke: ";
		getline(cin, viTri);

		int stt = DemSoSachCon(b.dms) + 1;
		for (int i = 0; i < soLuong; ++i) {
			PTR_DanhMucSach p = new DanhMucSach;
			p->data.MaSach = b.ISBN + "_" + to_string(stt++);
			p->data.TrangThai = 0; // 0: Cho mượn được
			p->data.ViTri = viTri;
			p->next = nullptr;

			if (b.dms == nullptr) {
				b.dms = p;
			} else {
				PTR_DanhMucSach cur = b.dms;
				while (cur->next != nullptr) cur = cur->next;
				cur->next = p;
			}
		}
		cout << "=> Da tu dong danh ma cho " << soLuong << " cuon sach!\n";
}
void ThemDauSach(DS_DauSach *ds, DauSach b){
    if (ds->n >= MAXSACH) {
        cout << "(!) Danh sach dau sach da day, khong the them!\n";
        return;
    }

    DauSach* pMoi = new DauSach;
    *pMoi = b;

    int pos = ds->n;
    // Chèn có thứ tự tăng dần theo TenSach
    while (pos > 0 && ds->ds[pos - 1]->TenSach > pMoi->TenSach) {
        ds->ds[pos] = ds->ds[pos - 1];
        pos--;
    }
    ds->ds[pos] = pMoi;
    ds->n++;
    cout << "=> Them dau sach co thu tu thanh cong!\n";
}

//Cau D: InDS_DauSach theo the loai va tang dan theo ten sach
void InDS_DauSach(DS_DauSach *ds){
	if (ds->n==0){
		cout<<"\nThu vien chua co dau sach nao";
		return;
	}
	string tlDaIn[MAXSACH];
	int slTL=0;

	for (int i=0; i<ds->n;i++){
		bool daCo = false;
		for (int j=0;j<slTL;++j){
			if (stricmp(ds->ds[i]->TheLoai.c_str(),tlDaIn[j].c_str()) == 0) {
               	daCo = true;
                	break;
            	}
        	}
        	if (!daCo) {
            	tlDaIn[slTL++] = ds->ds[i]->TheLoai;
		}
	}

	for (int t = 0; t < slTL; ++t) {
        cout << "\n================ THE LOAI: " << tlDaIn[t] << " ================\n";
        cout << left << setw(15) << "ISBN"
             << setw(35) << "TEN SACH"
             << setw(10) << "SO TRANG"
             << setw(20) << "TAC GIA"
             << setw(10) << "NAM XB" << "\n";
        cout << string(90, '-') << "\n";

        for (int i = 0; i < ds->n; ++i) {
            if (stricmp(ds->ds[i]->TheLoai.c_str(), tlDaIn[t].c_str()) == 0) {
                cout << left << setw(15) << ds->ds[i]->ISBN
                     << setw(35) << ds->ds[i]->TenSach
                     << setw(10) << ds->ds[i]->SoTrang
                     << setw(20) << ds->ds[i]->TacGia
                     << setw(10) << ds->ds[i]->NamXuatBan << "\n";
            }
        }
	}
}

// ===================== CÂU E =====================
void TimSachTheoTen(DS_DauSach DS, PTR_DanhMucSach DMS, string TenSach){
	bool timThay = false;
    for (int i = 0; i < DS.n; i++) {
        if (DS.ds[i]->TenSach.find(TenSach) != string::npos) {
            timThay = true;
            cout << "\nISBN: " << DS.ds[i]->ISBN
                 << " | Ten sach: " << DS.ds[i]->TenSach
                 << " | Tac gia: " << DS.ds[i]->TacGia
                 << " | Nam XB: " << DS.ds[i]->NamXuatBan
                 << " | The loai: " << DS.ds[i]->TheLoai << "\n";
            cout << left << setw(15) << "MA SACH" << setw(20) << "TRANG THAI" << setw(20) << "VI TRI" << "\n";
            for (PTR_DanhMucSach p = DS.ds[i]->dms; p != NULL; p = p->next) {
                string tt = (p->data.TrangThai == 0) ? "Cho muon duoc" : 
                            ((p->data.TrangThai == 1) ? "Da muon" : "Da thanh ly");
                cout << left << setw(15) << p->data.MaSach << setw(20) << tt << setw(20) << p->data.ViTri << "\n";
            }
        }
    }
    if (!timThay) cout << "Khong tim thay dau sach phu hop!\n";
}

//Câu F: Mượn sách
void MuonSach(PTR_TheDocGia &root, DS_DauSach &DS){
	PTR_TheDocGia p; int MaThe;

	do{
		cout << "Nhập mã thẻ độc giả: "; cin >> MaThe; cin.ignore();
		p = SearchDocGia(root, MaThe);
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
		p = SearchDocGia(root, MaThe);
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
	PTR_TheDocGia p = SearchDocGia(root, Mathe);
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

//Câu J: Top n sách được mượn
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
void TopSach(DS_DauSach *DS,int soluong, PTR_TheDocGia root){
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
	cout << "========================TOP "<< soluong << "SÁCH ĐƯỢC MƯỢN NHIỀU NHẤT========================" << endl;
	cout << left << setw(5) << "STT" << setw(20) <<"MÃ ISBN" << setw(50) << "TÊN SÁCH" << setw(50) <<"TÁC GIẢ" << setw(15) << "THỂ LOẠI" << setw(15) << "SỐ LƯỢT MƯỢN" << endl;
	
	
	for (int i = 0; i<soluong; i++){
		cout << left << setw(5) << i+1 
			 << setw(20) <<DSM[i].ds->ISBN
			 << setw(50) << DSM[i].ds->TenSach
			 << setw(50) << DSM[i].ds->TacGia
			 << setw(15) << DSM[i].ds->TheLoai
			 << setw(15) << DSM[i].TongLanMuon<< endl;
	}
}

void ReadDauSach(DS_DauSach *DS){
	ifstream fin("DAUSACH.txt");
	if(!fin.is_open()){
		cout << "Khong mo duoc file!" << endl;
		return;
	}
	string s;
	while (getline(fin,s)){
		if(s == "") continue;
		DauSach ds;
		int i = 0;

		while (i < s.length() && s[i] != '|')
			ds.ISBN += s[i++];
		i++;
		while (i < s.length() && s[i] != '|')
			ds.TenSach += s[i++];
		i++;
		while (i < s.length() && s[i] != '|')
			ds.TacGia += s[i++];
		i++;
		while (i < s.length() && s[i] != '|')
			ds.TheLoai += s[i++]; 
		i++;

		ds.SoTrang = 0; ds.NamXuatBan = 0;

		while (i < s.length() && s[i] != '|')
			ds.SoTrang = ds.SoTrang*10 + (s[i++] - '0');
		i++;
		while (i <s.length() && s[i] != '|')
			ds.NamXuatBan = ds.NamXuatBan*10 +(s[i++] - '0');
		i++;
		PTR_DanhMucSach pre = NULL;
		ds.dms = NULL;

		while (i <s.length()){
			PTR_DanhMucSach p = new DanhMucSach;
			p->next = NULL;

			if(s[i] == ';')i++;

			while (i < s.length() && s[i] != ',')
				p->data.MaSach += s[i++];
			i++;
			while (i < s.length() && s[i] != ',')
				p->data.TrangThai = s[i++] - '0';
			i++;
			while (i < s.length() && s[i] != ';')
				p->data.ViTri += s[i++];
			
			if(pre == NULL){
				ds.dms = p;
			}
			else{
				pre->next = p;
			}
			pre = p;
		}
		ThemDauSach(DS, ds);
	}
	fin.close();
}

int main(){
	DS_DauSach DS;
	PTR_TheDocGia root = NULL;
	
	return 0;
}
