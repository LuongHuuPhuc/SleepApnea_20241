# PPG_PCG_project_20241
***Mục đích dự án***
* Project này có mục đích đánh giá chất lượng sinh học nhờ vào tiếng hiệu PCG và PPG thông qua tiếng ngáy và chỉ số SpO2 trong máu
* Qua đó có thể nhận biết được tình trạng sức khỏe của người khảo sát
* Dự án thực hiện để chuẩn đoán người mắc chứng "Ngưng Thở" (Sleep Apnea) khi ngủ
***
***Hardware và Software***
1. MCU Esp32
   * 4MB bộ nhớ Flash
   * 448KB bộ nhớ ROM
   * 520KB bộ nhớ SRAM
   * Thạch anh giao động 40MHz 
   * Hộ trợ nhiều chuẩn giao tiếp khác nhau (I2C, SPI, I2S,...)
   ![image](https://github.com/user-attachments/assets/62cba73e-3d17-49de-bc7a-c4a977da22d7)
2. Cảm biến INMP441
   * Độ phân giải dữ liệu có thể thu được là 24-bit
   * Hỗ trợ 2 kênh dữ liệu trái phải (MONO và STEREO), mỗi kênh 32-bit dữ liệu
   * Giao tiếp theo chuẩn I2S (Inter IC Sound)
   * Tần số lấy mẫu khả dụng từ 8kHz - 192kHz
  ![image](https://github.com/user-attachments/assets/acaae52e-ac4d-4ab9-967f-c5e09cc1c63d)
4. Cảm biến MAX30102
   * Độ phân giải phụ thuộc vào bộ ADC, tối đa lên tớ 18-bit
   * Giao tiếp theo chuẩn I2C (Inter Integ_PCG_project_20241
***Mục đích dự án***
* Project này có mục đích đánh giá chất lượng sinh học nhờ vào tiếng hiệu PCG và PPG thông qua tiếng ngáy và chỉ số SpO2 trong máu
* Qua đó có thể nhận biết được tình trạng sức khỏe của người khảo sát
* Dự án thực hiện để chuẩn đoán người mắc chứng "Ngưng Thở" (Sleep Apnea) khi ngủ
***
***Hardware và Software***
1. MCU Esp32PG_PCG_project_20241
***Mục đích dự án***
* Project này có mục đích đánh giá chất lượng sinh học nhờ vào tiếng hiệu PCG và PPG thông qua tiếng ngáy và chỉ số SpO2 trong máu
* Qua đó có thể nhận biết được tình trạng sức khỏe của người khảo sát
* Dự án thực hiện để chuẩn đoán người mắc chứng "Ngưng Thở" (Sleep Apnea) khi ngủ
***
***Hardware và Software***
1. MCU Esp32
   * 4MB bộ nhớ Flash
   * 448KB bộ nhớ ROM
   * 520KB bộ nhớ SRAM
   * Thạch anh giao động 40MHz 
   * Hộ trợ nhiều chuẩn giao tiếp khác nhau (I2C, SPI, I2S,...)
   ![image](https://github.com/user-attachments/assets/62cba73e-3d17-49de-bc7a-c4a977da22d7)
2. Cảm biến INMP441
   * Độ phân giải dữ liệu có thể thu được là 24-bit
   * Hỗ trợ 2 kênh dữ liệu trái phải (MONO và STEREO), mỗi kênh 32-bit dữ liệu
   * Giao tiếp theo chuẩn I2S (Inter IC Sound)
   * Tần số lấy mẫu khả dụng từ 8kHz - 192kHz
  ![image](https://github.com/user-attachments/assets/acaae52e-ac4d-4ab9-967f-c5e09cc1c63d)
4. Cảm biến MAX30102
   * Độ phân giải phụ thuộc vào bộ ADC, tối đa lên tớ 18-bit
   * Giao tiếp theo chuẩn giao thức truyền thông nối tiếp I2C (Inter Integrated Circuit)
   * Hoạt động tốt ở dải 0,5Hz - 5Hz
   * Tần số lấy mẫu tối đa từ 50Hz - 3200Hz
   * Tần số phù hợp để đo tín hiệu sinh học là 100Hz - 400Hz
 ![image](https://github.com/user-attachments/assets/7098881a-031f-4aa7-83fe-95745adaab84)
5. Module thẻ nhớ SD
   * Sử dụng giao thức truyền thông đồng bộ SPI (Serial Peripheral Interface)
![image](https://github.com/user-attachments/assets/9d934c58-a4e7-4051-beb1-621e56522a6a)
6. Màn hình OLED 0.96 inch SSD1306
   * Sử dụng giao thức truyền thông nối tiếp I2C
   * Số điểm hiển thị 128x64 điểm
   * Driver: SSD1306
  ![image](https://github.com/user-attachments/assets/e013fb40-f03b-4b9e-ad67-e852a17fe478)
7. Phần mềm hỗ trợ lập trình firmware
   * Framework ESP-IDF
   * Arduino IDE
![image](https://github.com/user-attachments/assets/dca27ee6-34d8-45f9-b89f-c12095224862)




