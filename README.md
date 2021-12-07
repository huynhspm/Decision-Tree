# DecisionTree
## Bài Tập Lớn Decision Tree môn Cấu trúc dữ liệu và giải thuật kỳ 1 năm học 2021-2022.
## Thành viên: Trịnh ngọc Huỳnh MSV 20020039.
## Giáo viên hướng dẫn: CN Nguyễn Đình Tuân.
***
## 1. Giới thiệu bộ dữ liệu và ouput:
### Input:
-Bộ dữ liệu về Balance-Scale gồm 4 thuộc tính và 3 nhãn L(Left), R(Right), B(Balance).
- File train.txt có 439 bản ghi dùng để build decision tree.
- File valid.txt có 93 bản ghi dùng để test decision.tree.
- File Decision Tree.pptx giới thiệu về những đặc điểm cơ bản Decision Tree, GiniScore, Entropy, Split Gain, Ratio Gain, Confusion Matrix, Hyperparameter Search, Cross Validation.
### Output:
- Với file train.txt ta sẽ xây decision tree và test thử trên valid. Test cuối cùng là thầy test lại bằng file của thầy
- Print tree: in ra cây quyết định.
- Max Depth: Giá trị maxDepth chọn thông qua Hyperparameter search.
- Min Size: Giá trị minSize chọn thông qua Hyperparameter search.
- Accuracy of train: accuracy khi dự đoán chính train đã xây cây.
- Accuracy of valid: accuracy khi test thử trên valid.
- confusionMatrix: Bảng 3x3 với cột là dự doán. Cột là true label, hàng là predict label. 
### 2. Giới thiệu thuật toán, module implement:
- Decision Trê: xây dựng cây quyết định theo cách tham lam và đệ quy từ gốc xuống lá
- Hyperparameter search: chạy các trường hợp minSize và maxDepth từ đó đánh giá giá trị minSize và maxDepth tốt nhất.
- Cross validation: Chia train ra làm phần để build và phần test. Cross validation sẽ chia đều cho các phần rồi tính giá trị trung bình. Từ đó sẽ khách quan hơn, giảm overfitting.
- Confusion Matrix: Bảng đối chiếu nxn với n là số thuộc tính để biết được số lượng nhãn true label, nhãn wrong label và đoán sai nhãn nào thành nhãn nào
### 3. Giới thiệu cấu trúc:
- source code ở file decisionTree.cpp.
- struct Node: 
      +) lưu lại index ở attribute và giá trị của attribute để chia tập data thành 2 phần
      +) 2 Node con left, right
      +) label: nếu là node lá được gán nhãn
- struct Data: lưu lại dữ liệu với label và tập attribute tương ứng
- getGiniEachGroup: tính gini score của 1 tập data
- getGiniSplit: tính giniSplit dựa vào Node cha và 2 Node con
- getEntropyEachGroup: tính entropy của 1 tập data
- getGainSplit: tính gainSplitdựa vào Node cha và 2 Node con
- getGainRatio: tính gainRatio dựa vào Node cha và 2 Node con
- splitData: chia data thành 2 phần dựa vào index của attrivute và value của attribute mang ra chia
- findLabel: tìm label có tỉ lệ cao nhất trong data
- buildTree: 
       +) xây decision tree theo cách đệ quy;
       +) sử dụng maxDepth, MinSize, giniScore làm điều kiện dừng
       +) chạy hyperparameter để tham làm tìm index của attribute và value của attribute để chia data
 - checkData: tìm label decision Tree đoán được từ tập attribute của nó
 - calculateAccuracy: tỉ lệ đoán đúng của decision tree với tập data đầu vào
 - showTree: print tree theo preoder 
 - showWrongData: print những dữ liệu bị đoán sai
 - showConfusionMatrix: in confusion matrix
 - inputData: đọc dữ liệu từ file txt 
 - crossValidation: chia tập train thành k phần 
 - findDataTrainDataTest: chọn trọn k phần 1 phần để test thử và k - 1 phần còn lại để build tree
 ***
 ## Update 1: chạy Hyperparameter tìm minSize, maxDepth tối ưu cùng với sử dụng GiniSplit để tham lam
 - for maxDepth và minSize trong khoảng 1 -> 10 vì lớn hơn 10 thì accuracy không tăng nữa
 - **Cải thiện: Thay vì chọn ngẫu nhiên Minsize, Maxdepth thì đã cố định được tốt nhất và tránh được overfitting khi chọn. 
 - **Accuracy in validTest: 83% và Accuracy in trainTest: 80% với maxDepth = 6, MinSize = 7.
 ***
 ## Update 2: thay thể GiniSplit bằng gainSplit và gainRatio cùng với entropy để tham lam
 - cách sử dụng và cách tính xem tai: https://bigdatauni.com/tin-tuc/thuat-toan-cay-quyet-dinh-p-3-c4-5-entropy.html
 - **Accuracy in validTest: 85% và Accuracy in trainTest: 76% với maxDepth = 9, MinSize = 6 - gainRatio.
 - **Accuracy in validTest: 86% và Accuracy in trainTest: 77% với maxDepth = 5, MinSize = 3 - gainSplit.
 ## Update 3: thêm điều kiện dừng khi Node đã pure.
 - Khi Node có giniScore = 0 hay toàn bộ data cùng label thì không chia nữa
 - **Cải thiện : Giảm độ cao của cây
 ## Updata 4: sử dụng cross validation để chia train thành 2 tập để build và test
 - Do data ít: có thể một số điểm dữ liệu có ích cho qúa trình train đã bị bạn ném vào để làm validation, test và model không có cơ hội học điểm dữ liệu đó. Thậm chí, đôi khi do ít dữ liệu nên có một vài class chỉ có trong validation, test mà không có trong train (do việc chia train, val là hoàn toàn ngẫu nhiên) dẫn đến một kết quả tồi tệ khi validation và test. Và nếu chúng ta dựa ngay vào kết quả đó để đánh giá rằng model không tốt thì thật là oan uổng cho nó giống như một học sinh không được học Tiếng Anh mà phải đi thi TOEFL vậy =))
 - **Cải thiện: Accuracy in validTest: 86% và Accuracy in trainTest: 83% với maxDepth = 8, MinSize = 3 - sử dụng gainSplit.
