import matplotlib.pyplot as plt

# Đọc dữ liệu từ tệp CSV hoặc đầu vào
def read_data_from_file(filename):
    inmp441_data = []
    red_data = []
    ir_data = []

    with open(filename, 'r') as file:
        for line in file:
            try:
                # Tách dữ liệu dựa trên dấu phẩy
                inmp441, red, ir = line.strip().split(',')
                inmp441_data.append(int(inmp441))
                red_data.append(int(red))
                ir_data.append(int(ir))
            except ValueError:
                print(f"Invalid line: {line.strip()}")

    return inmp441_data, red_data, ir_data

# Vẽ đồ thị
def plot_data(inmp441_data, red_data, ir_data):
    plt.figure(figsize=(12, 6))

    # Vẽ dữ liệu INMP441
    plt.plot(inmp441_data, label='INMP441', color='blue', linestyle='-')
    
    # Vẽ dữ liệu Red
    plt.plot(red_data, label='MAX30102 Red', color='red', linestyle='--')
    
    # Vẽ dữ liệu IR
    plt.plot(ir_data, label='MAX30102 IR', color='green', linestyle='-.')
    
    # Thêm tiêu đề và nhãn
    plt.title("INMP441 and MAX30102 Data Visualization")
    plt.xlabel("Sample Index")
    plt.ylabel("Amplitude")
    plt.legend()
    plt.grid(True)

    # Hiển thị đồ thị
    plt.show()

# Đường dẫn tới tệp CSV
filename = "D:\Esp-idf\Mysource\inm_max_code\data_text\data_inm_max.csv"  # Thay "data.csv" bằng đường dẫn tới tệp dữ liệu của bạn

# Đọc và hiển thị đồ thị
inmp441_data, red_data, ir_data = read_data_from_file(filename)
plot_data(inmp441_data, red_data, ir_data)
