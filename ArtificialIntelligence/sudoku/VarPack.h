#ifndef VAR_PACK_H
#define VAR_PACK_H

//VarPack類,版本1.0,作者:何超越
//作用簡介:用於不同類型的變數或者資料打包，便於傳送
//----------------------------背景說明-------------------------------
//在調用函數和返回函數傳遞參數的時候，往往參數不止一個，而C語言的語法規
//定在返回參數時，只能返回一個整體。這樣會帶來很多不方便，通常情況下的解
//決方法是定義一個結構體，將所有的返回參數打包。這樣確實能夠解決問題但
//是也存在很多缺點:
//
//1.對於不同的類型需要定義不同的結構體，而這些結構體僅用於傳遞參數，結構
//體種類過多使得程式變複雜，可讀性和可維護性都會受到比較大的影響。
//
//2.返回結構體時需要一次進棧和出棧的操作，如果結構體大小偏大，對程式性能
//都有影響。
//
//3.當需要返回的資料量變化的時候，比如要返回的是不等長度的陣列，普通結構
//體就無法完成這個工作，如果在結構體中含指標就會涉及到深淺拷貝的問題，這
//樣程式會比較複雜，可讀性和可維護性都會受到比較大的影響。
//
//針對上述的缺點，在這裡定義一個類，它能夠隨意存入各種類型的資料，也能夠
//釋放這些資料，在進棧和出棧的時候不需要內容的複製。在操作上簡便易行，直
//觀易懂。它的出現能夠簡化程式，具有比較好的可讀性和可維護性。
//
//同時它也是有缺點的:
//
//1.效率可能會受到影響，在存入資料和釋放資料的時候，會檢查緩存大小和讀取
//指針等等操作，這些操作會降低存入和釋放資料的速度。
//
//2.語法結構不夠嚴密，沒有變數類型的嚴格檢查，如果存入和釋放資料順序不對，
//不會有明顯的錯誤警報。
//---------------------------使用說明-基本------------------------------
//1.創建VarPack包:
//VarPack vp;//創建默認VarPack包，緩存大小1024byte
//VarPack vp(500);//創建制定指定的VarPack包，緩存大小500byte，注意一旦包
//的大小確定，就不可再更改。
//提示：VarPack包的大小最好要大一點，以便於保證能夠存放這麼多資料。
//
//2.利用VarPack包存資料
//VarPack vp(50);//創建大小為50byte的包
//vp << 1 << 1.0/3 << ';' << "Hello World!";//向包內填入資料,順序為1(int)，
//1.0(double)，';'(char ), "Hello World!"(char array).
//int ix,double dx, char cx, char cax[100];//定義一些變數
//vp >> ix >> dx >> cx >> cax;//釋放包內資料,順序同上
//cout << ix << dx << cx << cax;//列印結果(10.333333;Hello World!)
//提示：模仿了iostream中的輸入輸出流，不過<<表示填充資料,>>表示釋放資料
//
//3.利用VarPack傳遞參數
//VarPack function(VarPack a);
//int main()
//{
//	VarPack vp1,vp2;
//	vp1 = function(vp2);
//}
//提示：在傳遞VarPack變數的時候就和普通變數一樣,可以作為形參和返回參數。
//
//---------------------------使用說明-高級------------------------------
//1.批量存入和讀出資料
//int arrayA[100],arrayB[100];
//vp.Write((void *)arrayA, 100*sizeof(int));//將arrayA寫入vp中
//vp.Read((void *)arrayB, 100*sizeof(int));//從vp中讀出資料,存入arrayB中
//提示,和fstream的用法很相似
//
//2.VarPack中的副本和傳遞問題
//在VarPack對象作為形參或者返回參數的時候，它往往會被拷貝好幾次。為了
//保證傳遞的效率，使用了淺拷貝，即單純的複製指標而不複製內容。這樣的結果
//就是有多個VarPack物件指向一個緩存區域，為了保證這些物件被銷毀時不進行
//重複的刪除記憶體操作，就會定義一個變數對其拷貝份數進行計數。當拷貝一次時
//計數加1，刪除一次時計數減1，減到0則刪除此緩存。
//因此在VarPack中物件的拷貝操作只是簡單的創建一個引用而已(下面稱之為引用)
//，若想創建一個拷貝物件並且擁有另一個緩存區域(下面稱之為副本)，可以使用
//MakeCopy()函數。
//VarPack a,b;//創建兩個物件,假設b緩存位址為0x0040f6b8
//a=b;//a是b的引用，a的緩存地址為0x0040f6b8
//a=b.MakeCopy();//a是b的副本，a的緩存地址為0x0040f952
//
//3.VarPack中的許可權問題
//VarPack物件由於存在多個引用物件，這些引用物件指向一個緩存區域，那麼對於
//緩存的寫操作可能會影響其他的引用物件。由於寫操作是追加型的，因此在眾多
//引用物件中可以且只能有1個物件有寫的許可權。為確保資料安全，只有原始的物件
//擁有寫的許可權，以後所有的引用物件只有讀的許可權。副本由於指向新的記憶體區域
//可以同時擁有讀和寫的許可權。
class VarPack
{
public:
	VarPack();
	VarPack(int maxdatalen);
	VarPack(VarPack &cp);
	~VarPack();
	VarPack MakeCopy();
public:
	VarPack & operator = (VarPack &cp);
public:
	void clear();
	bool good();
	bool eof();
public:
	void Read(void *t, int buflen);
	void Write(void *t, int buflen);
	template<class T>
	friend VarPack & operator << (VarPack & vp, T & t);
	template<class T>
	friend VarPack & operator >> (VarPack & vp, T & t);
	friend VarPack & operator << (VarPack & vp, const char *str);
	friend VarPack & operator >> (VarPack & vp, char *str);
private:
	int m_datalength;
	int m_pbegin;
	int m_pend;
	int m_author;
	int  *m_nlockbuf;
	char *m_buf;
};


template<class T>
VarPack & operator << (VarPack & vp, T & t)
{
	vp.Write((void *)&t, sizeof(T));
	return vp;
}

template<class T>
VarPack & operator >> (VarPack & vp, T & t)
{
	vp.Read((void *)&t, sizeof(T));
	return vp;
}

#endif//VAR_PACK_H
