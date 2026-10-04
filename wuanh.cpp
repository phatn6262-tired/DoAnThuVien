#include <bits/stdc++.h>
using namespace std;

const long long MAXSACH = 10000;

struct ngaythang{
	int ngay, thang, nam;
}

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

// wuanhdeptrai (danh sach the doc gia)
struct TheDocGia{
	int MaThe;
	char ho[40];
	char ten[20];
	char gioitinh[5];
	int TrangThaiThe;
	PTR_MuonTra mt;

	TheDocGia(){
		MaThe =0;
		ho[0]='\0';
		ten[0]='\0';
		gioitinh[0]='\0';
		TrangThaiThe = 1;
		mt=nullptr;
	}

	TheDocGia(int _ma, const char* _ho, const char* _ten, const char* _gioitinh, int _tt, PTR_MuonTra _mt){
        MaThe = _ma;
        strcpy(ho, _ho);
        strcpy(ten, _ten);
        strcpy(gioitinh, _gioitinh);
        TrangThaiThe = _tt;
        mt = _mt;
    }
};

struct DS_TheDocGia{
	TheDocGia data;
	DS_TheDocGia *left, *right;
};
typedef DS_TheDocGia *TREE_TheDocGia;

// cay the doc gia
TREE_TheDocGia SearchDocGia(TREE_TheDocGia root, int MaThe){
	if (root== nullptr || root ->data.MaThe== MaThe){
		return root;
	}
	if (MaThe < root ->data.MaThe){
		return SearchDocGia (root->left,MaThe);
	}
	return SearchDocGia(root->right, MaThe);
}

int SinhMaThe(TREE_TheDocGia root){
	int ma;
	do {
		ma=rand()% 90000+10000;
	}while (SearchDocGia(root, ma) != nullptr);
	return ma;
}

bool InsertDocGia(TREE_TheDocGia &p, const TheDocGia &TDG){
	if (p == nullptr){
		p = new DS_TheDocGia;
		p ->data = TDG;
		p ->left= p->right = nullptr;
		return true;
	}
	if (TDG.MaThe <p->data.MaThe){
		return InsertDocGia(p ->left, TDG);
	}
	if (TDG.MaThe > p ->data.MaThe){
		return InsertDocGia(p -> right, TDG);
	}
	return false;
}

void TimLop(DS_TheDocGia*& p, DS_TheDocGia*& q){
	if (q ->left != nullptr){
		TimLop(p,q->left);
	} 
	else {
		p->data = q->data;
		p=q->right;
	}
}

bool DelDocGia(TREE_TheDocGia& p, int MaThe){
	if (p==nullptr) return false;
	if (MaThe < p->data.MaThe){
		return DelDocGia(p ->left,MaThe);
	}
	else if (MaThe > p->data.MaThe){
		return DelDocGia(p->right, MaThe);
	}
	else{
		for (TREE_TheDocGia cur = p->data.mt; cur != nullptr; cur = cur->next) {
            if (cur->data.TrangThaiThe == 0) {
                cout << "Doc gia dang muon sach chua tra, 0 xoa the!\n";
                return false;
	}
	DS_TheDocGia* temp = p;
	if (p->left==nullptr) p= p->right;
	else if (p->right==nullptr) p =p->left;
	else TimLop(temp, p->right);
	delete temp;
	return true;
}

//Cau A ****chua chinh thuc, chua lam giao dien

void ThemTheDocGia(TREE_TheDocGia& root){
	TheDocGia dg;
	dg.MaThe = SinhMaThe(root);
	cout<<"\n them thanh cong \n";
	cout<< " ma the "<< dg.MaThe <<"\n";
	cin.ignore();
	cout <<"nhap ho:"; cin.getline(dg.ho,40);
	cout <<"nhap ten:"; cin.getline(dg.ten,20);

	do{
		cout<<" gioi tinh (Nam/Nu):";
		cin.getline (dg.gioitinh,5);
		if (stricmp(dg.gioitinh,"Nam")!=0 && stricmp(dg.gioitinh,"Nu")!=0){
			cout<<"chi duoc nhap Nam hoc Nu: \n";
		}
	}while (stricmp(dg.gioitinh,"Nam")!=0 && stricmp(dg.gioitinh,"Nu")!=0);

	dg.TrangThaiThe =1;
	dg.mt = nullptr;
	InsertDocGia(root, dg);
	cout <<"them doc gia thanh cong";
}

void HieuChinhDocGia(TREE_TheDocGia root){
	int mathe;
	cout <<"\n nhap ma the can chinh";
	cin>> mathe;
	DS_TheDocGia* p= SearchDocGia(root, maThe);
	if (!p){
		cout <<"ko tim thay";
		return;
	}
	cin.ignore();
	cout<<" nhap ho:";
	cin.getline(p->data.ho, 40);
	cout <<" nhap ten:";
	cin.getline(p->data.ten, 20);

	do{
		cout <<"nhap gioi tinh (Nam/Nu):";
		cin.getline(p->data.gioitinh, 5);
    } while (stricmp(p->data.gioitinh, "Nam") != 0 && stricmp(p->data.gioitinh, "Nu") != 0);

    cout << "Trang thai the (0: Khoa, 1: Hoat dong): ";
    cin >> p->data.TrangThaiThe;
    cout << "=> Cap nhat the thanh cong!\n";
}
	}
}
void XoaDocGia(TREE_TheDocGia &root){
	int maThe;
    cout << "\nNhap ma the can xoa: ";
    cin >> maThe;
    if (DelDocGia(root, maThe))
        cout << "=> Da xoa the doc gia thanh cong!\n";
    else
        cout << "(!) Xoa the that bai!\n";
} 

//Cau B chua chinh thuc con chinh them
void chuyenCayVaoMang(TREE_TheDocGia root, TheDocGia* arr[], int& count) {
    if (root == nullptr) return;
    chuyenCayVaoMang(root->Left, arr, count);
    arr[count++] = &(root->data);
    chuyenCayVaoMang(root->Right, arr, count);
}

//void SortByName(TREE_TheDocGia root){}
//Tao cay moi voi key = ten

void InDS_DocGia(TREE_TheDocGia root){
    TheDocGia* arr[MAX_DAUSACH];
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
         << setw(15) << "gioi tinh" << "\n";
    cout << string(73, '-') << "\n";

	cout << left << setw(10) << dg.maThe
         << setw(25) << dg.ho
         << setw(15) << dg.ten
         << setw(8) << dg.gioitinh
         << setw(15) << (dg.trangThaiThe == 1 ? "Hoat dong" : "Bi khoa") << "\n";
}
/// thầy mới làm chức năng cơ bản chưa quăng lên ai check nữa đang hơi bận xíu mấy đứa đợi thầy 

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