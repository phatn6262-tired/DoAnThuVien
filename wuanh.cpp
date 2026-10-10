#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

const long long MAXSACH = 10000;

// ===================== THAO TÁC NGÀY THÁNG =====================
struct NgayThang {
	int Ngay;
	int Thang;
	int Nam;
};

void InNgayThang(NgayThang nt) {
	cout << nt.Ngay << "/" << nt.Thang << "/" << nt.Nam;
}

int NamNhuan(int Nam) {
	if (Nam % 400 == 0 || (Nam % 4 == 0 && Nam % 100 != 0)) {
		return 1;
	}
	return 0;
}

int SoNgayTrongThang(int Thang, int Nam) {
	if (Thang == 4 || Thang == 6 || Thang == 9 || Thang == 11)
		return 30;
	else if (Thang == 2) {
		if (NamNhuan(Nam)) return 29;
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

int SoNgay(NgayThang NgayMuon, NgayThang NgayHienTai) {
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

// ===================== DANH MỤC SÁCH & ĐẦU SÁCH =====================
struct ThongTinSach { // Node cho DanhMucSach
	string MaSach;
	int TrangThai;
	string ViTri;
};

struct DanhMucSach {
	ThongTinSach data;
	DanhMucSach* next;
};
typedef DanhMucSach *PTR_DanhMucSach;

struct DauSach {
	string ISBN, TenSach, TacGia, TheLoai;
	int SoTrang, NamXuatBan;
	PTR_DanhMucSach dms = NULL;
};

struct DS_DauSach {
	DauSach* ds[MAXSACH];
	int n = 0;
};

PTR_DanhMucSach TimSach(DS_DauSach DS, string MaSach) {
	for (int i = 0; i < DS.n; i++) {
		DauSach *p = DS.ds[i];
		PTR_DanhMucSach q;
		for (q = p->dms; q != NULL; q = q->next) {
			if (MaSach == q->data.MaSach) {
				return q;
			}
		}
	}
	return NULL;
}

// ===================== MƯỢN TRẢ =====================
struct MuonTra { // Node cho DS_MUONTRA
	string MaSach;
	NgayThang NgayMuon, NgayTra;
	int TrangThai;
};

struct DS_MuonTra {
	MuonTra data;
	DS_MuonTra *next;
};
typedef DS_MuonTra *PTR_MuonTra;

void CapNhatSachMuon(PTR_MuonTra &n, string MaSach) {
	n->data.MaSach = MaSach;
	n->data.NgayMuon = LayNgayHienTai();
	n->data.NgayTra = {0, 0, 0};
	n->data.TrangThai = 0;
	n->next = NULL;
}

// ===================== THẺ ĐỘC GIẢ (BST) =====================
struct TheDocGia {
	int MaThe;
	char ho[40];
	char ten[20];
	char phai[5];
	int TrangThaiThe;
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
typedef DS_TheDocGia *PTR_TheDocGia;

// Các thao tác cơ bản trên cây BST Độc giả
PTR_TheDocGia TimMaThe(PTR_TheDocGia &root, int MaThe) {
	if (root == NULL) return NULL;
	if (MaThe == root->data.MaThe) return root;

	if (MaThe < root->data.MaThe) return TimMaThe(root->left, MaThe);
	else return TimMaThe(root->right, MaThe);
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

bool DelDocGia(PTR_TheDocGia& p, int MaThe) {
	if (p == nullptr) return false;
	if (MaThe < p->data.MaThe) {
		return DelDocGia(p->left, MaThe);
	}
	else if (MaThe > p->data.MaThe) {
		return DelDocGia(p->right, MaThe);
	}
	else {
		for (PTR_MuonTra cur = p->data.mt; cur != nullptr; cur = cur->next) {
            if (cur->data.TrangThai == 0) {
                cout << "Doc gia dang muon sach chua tra, 0 xoa the!\n";
                return false;
            }
		}
		DS_TheDocGia* temp = p;
		if (p->left == nullptr) p = p->right;
		else if (p->right == nullptr) p = p->left;
		else TimLop(p, p->right);
		delete temp;
		return true;
	}
}

int coQuaHan(PTR_TheDocGia &p) {
	PTR_MuonTra q;
	NgayThang NgayHienTai = LayNgayHienTai();
	for (q = p->data.mt; q != NULL; q = q->next) {
		if (q->data.TrangThai == 0) {
			int tong = SoNgay(q->data.NgayMuon, NgayHienTai);
			if (tong > 7) return 1;
		}
	}
	return 0;
}

// In danh sách mượn trả của 1 độc giả
void InDS_MuonTra(PTR_TheDocGia &p, int &BookSum) {
	cout << left << setw(15) << "Mã sách" << setw(15) << "Ngày mượn" << setw(15) << "Ngày trả" << setw(15) << "Trạng thái" << endl;
	PTR_MuonTra q;
	for (q = p->data.mt; q != NULL; q = q->next) {
		cout << left << setw(15) << q->data.MaSach << setw(15);

		if (q->data.TrangThai == 0) {
			InNgayThang(q->data.NgayMuon);
			cout << left << setw(15) << "Chưa trả" << setw(15) << "Đang mượn" << endl;
			BookSum++;
		}
		else if (q->data.TrangThai == 1) {
			InNgayThang(q->data.NgayMuon);
			cout << left << setw(15); 
			InNgayThang(q->data.NgayTra); 
			cout << left << setw(15) << "Đã trả" << endl;
		}
		else {
			InNgayThang(q->data.NgayMuon);
			cout << left << setw(15) << "Chưa trả" << setw(15) << "Làm mất sách" << endl;
		}
	}
}

// ===================== CÂU A: THẺ ĐỘC GIẢ =====================
void ThemTheDocGia(PTR_TheDocGia& root) {
	TheDocGia dg;
	dg.MaThe = SinhMaThe(root);
	cout << "\n them thanh cong \n";
	cout << " ma the " << dg.MaThe << "\n";
	cin.ignore();
	cout << "nhap ho: "; cin.getline(dg.ho, 40);
	cout << "nhap ten: "; cin.getline(dg.ten, 20);

	do {
		cout << " gioi tinh (Nam/Nu): ";
		cin.getline(dg.phai, 5);
		if (stricmp(dg.phai, "Nam") != 0 && stricmp(dg.phai, "Nu") != 0) {
			cout << "chi duoc nhap Nam hoc Nu: \n";
		}
	} while (stricmp(dg.phai, "Nam") != 0 && stricmp(dg.phai, "Nu") != 0);

	dg.TrangThaiThe = 1;
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
	cin.getline(p->data.ho, 40);
	cout << " nhap ten: ";
	cin.getline(p->data.ten, 20);

	do {
		cout << "nhap gioi tinh (Nam/Nu): ";
		cin.getline(p->data.phai, 5);
    } while (stricmp(p->data.phai, "Nam") != 0 && stricmp(p->data.phai, "Nu") != 0);

    cout << "Trang thai the (0: Khoa, 1: Hoat dong): ";
    cin >> p->data.TrangThaiThe;
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

// ===================== CÂU B: IN DANH SÁCH ĐỘC GIẢ =====================
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
            int cmpTen = stricmp(arr[j]->ten, arr[minIdx]->ten);
            if (cmpTen < 0 || (cmpTen == 0 && stricmp(arr[j]->ho, arr[minIdx]->ho) < 0)) {
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
	         << setw(25) << arr[i]->ho
	         << setw(15) << arr[i]->ten
	         << setw(8) << arr[i]->phai
	         << setw(15) << (arr[i]->TrangThaiThe == 1 ? "Hoat dong" : "Bi khoa") << "\n";
    }
}

// ===================== CÂU C =====================
void SinhMaSach(DauSach &b);
void ThemDauSach(DS_DauSach *ds, DauSach b){}

// ===================== CÂU D =====================
void InDS_DauSach(DS_DauSach *ds){} // In theo the loai, trong the loai lai in theo ten tang dan

// ===================== CÂU E =====================
void TimSachTheoTen(PTR_DanhMucSach DMS, string TenSach){}

// ===================== CÂU F: MƯỢN SÁCH =====================
void MuonSach(PTR_TheDocGia &root, DS_DauSach &DS) {
	PTR_TheDocGia p; int MaThe;

	do {
		cout << "Nhập mã thẻ độc gia: "; cin >> MaThe; cin.ignore();
		p = TimMaThe(root, MaThe);
		if (p == NULL)
			cout << "Mã thẻ không hợp lệ, nhập lại!" << endl;
		else {
			if (p->data.TrangThaiThe == 0) {
				cout << "Thẻ đã bị khóa!" << endl;
				_getch();
				return;
			}
			int BookSum = 0;
			if (p->data.mt == NULL) cout << "Độc giả không mược cuốn sách nào!" << endl;
			else InDS_MuonTra(p, BookSum);
			int ch;
			do {
				if (BookSum >= 3) {
					cout << "Số sách đã mượn đạt giới hạn (3 cuốn)!";
					_getch(); return;
				}
				else if (coQuaHan(p) == 1) {
					cout << "Độc giả đang mượn sách quá hạn!";
					_getch();
					return;
				}
				else {
					string MaSach;
					PTR_DanhMucSach s;
					
					do {
						cout << "Nhập mã sách bạn muốn mượn: ";
						getline(cin, MaSach);
						s = TimSach(DS, MaSach);
						if (s == NULL) {
							cout << "Mã sách không tồn tại!" << endl;
						}
					} while (s == NULL);
					
					if (s->data.TrangThai != 0) {
						if (s->data.TrangThai == 1)
							cout << "Sách đang được mượn!" << endl;
						else
							cout << "Sách đã thanh lý!" << endl;
						_getch();
						return;
					}
					PTR_MuonTra n = new DS_MuonTra;
					CapNhatSachMuon(n, MaSach);
					if (p->data.mt == NULL)
						p->data.mt = n;
					else {
						PTR_MuonTra q = p->data.mt;
						while (q->next != NULL)
							q = q->next;
						q->next = n;
					}

					s->data.TrangThai = 1;
					cout << "Mượn sách thành công!" << endl;
					BookSum++;
				}
				if (BookSum >= 3) {
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

				if (ch == 0) return;
			} while (ch == 1);
		} 
	} while (p == NULL);
}

// ===================== CÂU G =====================
void TraSach(PTR_TheDocGia &root, PTR_DanhMucSach &DMS, int Mathe, int MaSach){}

// ===================== CÂU H =====================
void LietKeSachMuon(int Mathe, PTR_TheDocGia &p){}

// ===================== CÂU I =====================
int ThoiGianMuon(int NgayMuon){}
void INDS_QuaHan(PTR_TheDocGia &root){}

// ===================== CÂU J =====================
void Top10Sach(DauSach *ds, PTR_MuonTra MT){}

// ===================== MAIN =====================
int main() {
	PTR_TheDocGia root = NULL;
	PTR_DanhMucSach DMS = NULL;
	PTR_MuonTra MT = NULL;
	DS_DauSach DS;
	return 0;
}