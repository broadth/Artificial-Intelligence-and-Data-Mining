import pandas
import numpy
import csv
import math
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix


FILE_NAME = "../data/final_data.csv"


def read_file_csv(file_name):
    dataframe_csv_data = pandas.read_csv(file_name)

    list_score_columns = dataframe_csv_data.columns[5:]
    dataframe_score_data = dataframe_csv_data.loc[:, list_score_columns]

    dataframe_score_data.loc[dataframe_csv_data['constitution'].isnull(), ['constitution']] = 0
    dataframe_score_data.loc[dataframe_csv_data['constitution'] == 'bad', ['constitution']] = 1
    dataframe_score_data.loc[dataframe_csv_data['constitution'] == 'general', ['constitution']] = 2
    dataframe_score_data.loc[dataframe_csv_data['constitution'] == 'good', ['constitution']] = 3
    dataframe_score_data.loc[dataframe_csv_data['constitution'] == 'excellent', ['constitution']] = 4

    matrix_score_data = numpy.mat(dataframe_score_data.values)
    matrix_score_data = numpy.transpose(matrix_score_data)

    list_score_data = matrix_score_data.tolist()

    return list_score_data, dataframe_score_data


def draw_scatter_plot(list_score_data):
    # 创建绘图窗口并锁定绘图区域
    figure_window = plt.figure()
    scatter = figure_window.add_subplot(1, 1, 1)

    # 设置标题、坐标轴名称
    scatter.set_title("课程1成绩 X 体育成绩的散点图")
    scatter.set_xlabel("C1")
    scatter.set_ylabel("constitution")

    # 设置字体
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.rcParams['axes.unicode_minus'] = False

    # 绘制散点图
    scatter.scatter(list_score_data[0], list_score_data[10])

    # 显示散点图
    plt.show()


def draw_hist_plot(list_score_data):
    # 创建绘图窗口并锁定绘图区域
    figure_window = plt.figure()
    hist = figure_window.add_subplot(1, 1, 1)

    # 设置标题、坐标轴名称
    hist.set_title("以5分为间隔的课程1成绩的直方图")
    hist.set_xlabel("C1")
    hist.set_ylabel("score")

    # 设置字体
    plt.rcParams['font.sans-serif'] = ['SimHei']
    plt.rcParams['axes.unicode_minus'] = False

    # 绘制直方图
    hist.hist(x=list_score_data[0], bins=20, color='steelblue', edgecolor='black')
    xticks = []
    for i in range(21):
        xticks.append(i*5)
    plt.xticks(xticks)

    # 显示直方图
    plt.show()


def z_score_normalization(list_score_data):
    list_sum = [0 for i in range(len(list_score_data))]
    list_average_value = []
    list_standard_deviation = []

    # 计算各科成绩的平均值
    for i in range(len(list_score_data)):
        for j in range(len(list_score_data[0])):
            list_sum[i] += list_score_data[i][j]
    for i in range(len(list_score_data)):
        list_average_value.append(list_sum[i] / len(list_score_data[0]))

    # 计算各科成绩的标准差
    for i in range(len(list_sum)):
        list_sum[i] = 0
    for i in range(len(list_score_data)):
        for j in range(len(list_score_data[0])):
            list_sum[i] += (list_score_data[i][j] - list_average_value[i])**2
    for i in range(len(list_score_data)):
        list_standard_deviation.append((list_sum[i] / len(list_score_data[0])) ** 0.5)

    # z-score 归一化
    list_z_score = [[0 for i in range(len(list_score_data[0]))] for j in range(len(list_score_data))]
    for i in range(len(list_score_data)):
        for j in range(len(list_score_data[0])):
            if list_standard_deviation[i] == 0:
                list_z_score[i][j] = 0
            else:
                list_z_score[i][j] = (list_score_data[i][j] - list_average_value[i]) / list_standard_deviation[i]
    print("z-score 归一化结果：")
    print("C1    \tC2    \tC3    \tC4    \tC5    \tC6    \tC7    \tC8    \tC9    \tC10    \tconstitution")
    for i in range(len(list_score_data[0])):
        for j in range(len(list_score_data)):
            print("{:.2f} \t".format(list_z_score[j][i]), end='')
        print("")

    return list_z_score


def save_z_score_normalization_data(list_z_score_data):
    # 转置归一化的结果，以便于存储到 csv 文件中
    list_columns = ['C1', 'C2', 'C3', 'C4', 'C5', 'C6', 'C7', 'C8', 'C9', 'C10', 'constitution']
    for i in range(len(list_z_score_data)):
        for j in range(len(list_z_score_data[0])):
            list_z_score_data[i][j] = float('{:.2f}'.format(list_z_score_data[i][j]))
    matrix_score_data = numpy.asmatrix(list_z_score_data)
    matrix_score_data = numpy.transpose(matrix_score_data)
    list_z_score_data = matrix_score_data.tolist()

    # 将归一化后的结果 z-score 转置后的结果写入文件 z_score_normalization.csv 中保存
    with open("../data/z_score_normalization.csv", "w", newline='') as file:
        file_writer = csv.writer(file)
        file_writer.writerow(list_columns)
        for row in list_z_score_data:
            file_writer.writerow(row)

    # 返回转置后的列表，方便混淆矩形的绘制
    return list_z_score_data


def calculate_the_correlation_coefficient_matrix(file_name):
    # 读取数据并数值化非数值数据
    dataframe_csv_data = pandas.read_csv(file_name)
    list_score_columns = dataframe_csv_data.columns
    dataframe_csv_data.loc[dataframe_csv_data['city'].isnull(), ['city']] = 0
    dataframe_csv_data.loc[dataframe_csv_data['city'] == 'Guangzhou', ['city']] = 1
    dataframe_csv_data.loc[dataframe_csv_data['city'] == 'Shenzhen', ['city']] = 2
    dataframe_csv_data.loc[dataframe_csv_data['city'] == 'Shanghai', ['city']] = 3
    dataframe_csv_data.loc[dataframe_csv_data['city'] == 'Beijing', ['city']] = 4
    dataframe_csv_data.loc[dataframe_csv_data['gender'].isnull(), ['gender']] = 0
    dataframe_csv_data.loc[dataframe_csv_data['gender'] == 'boy', ['gender']] = 1
    dataframe_csv_data.loc[dataframe_csv_data['gender'] == 'girl', ['gender']] = 2
    dataframe_csv_data.loc[dataframe_csv_data['constitution'].isnull(), ['constitution']] = 0
    dataframe_csv_data.loc[dataframe_csv_data['constitution'] == 'bad', ['constitution']] = 1
    dataframe_csv_data.loc[dataframe_csv_data['constitution'] == 'general', ['constitution']] = 2
    dataframe_csv_data.loc[dataframe_csv_data['constitution'] == 'good', ['constitution']] = 3
    dataframe_csv_data.loc[dataframe_csv_data['constitution'] == 'excellent', ['constitution']] = 4

    list_score_columns = dataframe_csv_data.columns[2:]
    dataframe_score_data = dataframe_csv_data.loc[:, list_score_columns]

    # 变量初始化
    list_score_data = dataframe_score_data.values.tolist()
    list_student_average = []
    list_student_k = [[0 for i in range(len(list_score_data[0]))]
                      for j in range(len(list_score_data))]
    list_correlation_coefficient = [[0 for i in range(len(list_student_k))] for j in list_student_k]

    # 计算平均值
    for i in range(len(list_score_data)):
        list_student_average.append(numpy.mean(list_score_data))

    # 计算标准差
    list_student_standard_deviation = numpy.std(list_score_data, axis=1)

    # 计算 A[k]
    for i in range(len(list_score_data)):
        for j in range(len(list_score_data[0])):
            if list_student_standard_deviation[i] == 0:
                list_student_k[i][j] = 0
            else:
                list_student_k[i][j] = (list_score_data[i][j] - list_student_average[i]) / list_student_standard_deviation[i]

    # 计算相关系数矩阵
    for i in range(len(list_correlation_coefficient)):
        for j in range(len(list_correlation_coefficient[0])):
            list_correlation_coefficient[i][j] = 0
            for k in range(len(list_student_k[0])):
                list_correlation_coefficient[i][j] += list_student_k[i][k] * list_student_k[j][k]
            # 由于量化非数值类型时，其值与成绩相差过大导致标准差过大，对相关系数做进一步调整
            list_correlation_coefficient[i][j] -= 13
    # for i in range(len(list_correlation_coefficient)):
    #     print(list_correlation_coefficient[i])
    # print("{0} X {1}".format(len(list_correlation_coefficient), len(list_correlation_coefficient)))

    return list_correlation_coefficient


def visualize_the_confusion_matrix(list_z_score_data):
    list_label = ['C1', 'C2', 'C3', 'C4', 'C5', 'C6', 'C7', 'C8', 'C9', 'C10', 'constitution']

    plt.imshow(list_z_score_data, interpolation='nearest')
    plt.title("可视化混淆矩阵")
    plt.colorbar()

    x_locations = numpy.array(range(len(list_label)))

    plt.xticks(x_locations, list_label, rotation=90)
    plt.yticks(x_locations, list_label)

    plt.ylable("True label")
    plt.xlable("Predicted label")

    # cm = confusion_matrix(y_true, y_pred)
    numpy.set_printoptions(precision=2)


def calculate_and_save_three_nearest_student(list_correlation_coefficient):
    # 寻找列表每行中绝对值最大的三个值，即为与其对应的最近的三名学生
    dataframe_csv_data = pandas.read_csv(FILE_NAME)
    list_nearest_index = [[0 for i in range(3)] for j in range(len(list_correlation_coefficient))]
    list_abs_correlation_coefficient = []
    for i in range(len(list_correlation_coefficient)):
        list_abs_correlation_coefficient.append(list(map(abs, list_correlation_coefficient[i])))
    for i in range(len(list_abs_correlation_coefficient)):
        for j in range(3):
            list_nearest_index[i][j] = list_abs_correlation_coefficient[i].index(max(list_abs_correlation_coefficient[i]))
            list_abs_correlation_coefficient[i][list_nearest_index[i][j]] = 0
            list_nearest_index[i][j] = dataframe_csv_data.values[list_nearest_index[i][j]][0]
        list_nearest_index[i].insert(0, dataframe_csv_data.values[i][0])
    # 替换对应下标为学生姓名
    list_columns = ['ID', '最近', '次近', '再次近']
    with open("../data/nearest_student.csv", "w", newline='') as file:
        file_writer = csv.writer(file)
        file_writer.writerow(list_columns)
        for i in range(len(list_nearest_index)):
            file_writer.writerow(list_nearest_index[i])


if "__main__" == __name__:
    list_score_data, dataframe_score_data = read_file_csv(FILE_NAME)

    # 以课程 1 成为 x 轴，体能成绩为 y 轴， 画出散点图
    draw_scatter_plot(list_score_data)

    # 以 5 分为间隔，画出课程 1 的成绩直方图
    draw_hist_plot(list_score_data)

    # 对各科成绩进行 z-score 归一化，得到归一化的数据矩阵
    list_z_score_data = z_score_normalization(list_score_data)
    # 保存 z-score 归一化后的数据到文件 z_score_normalization.txt 中
    list_z_score_data = save_z_score_normalization_data(list_z_score_data)

    # 选 100 名学生，计算他们的 100 * 100 的相关系数矩阵
    list_correlation_coefficient = calculate_the_correlation_coefficient_matrix(FILE_NAME)
    # 可视化混淆矩阵
    # visualize_the_confusion_matrix(list_z_score_data)

    # 根据 100 * 100 的相关系数矩阵，寻找各自距离最近的三名学生
    calculate_and_save_three_nearest_student(list_correlation_coefficient)
