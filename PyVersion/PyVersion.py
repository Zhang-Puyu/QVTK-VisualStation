import vtk
import pandas as pd

class Visual:
    def __init__(self):
        self.axes_actor = vtk.vtkAxesActor()
        self.stl_actor = vtk.vtkActor()
        self.points_actor = vtk.vtkActor()
        self.scalar_bar_actor = vtk.vtkScalarBarActor()

        self.renderer = vtk.vtkRenderer()

        self.render_window = vtk.vtkRenderWindow()
        self.render_window_interactor = vtk.vtkRenderWindowInteractor()

        # 设置坐标轴
        self.axes_actor.SetTotalLength(20, 20, 20)
        self.axes_actor.SetTipTypeToCone()
        self.axes_actor.AxisLabelsOn()

        self.axes_actor.GetXAxisCaptionActor2D().GetTextActor().SetTextScaleModeToNone()
        self.axes_actor.GetXAxisCaptionActor2D().GetCaptionTextProperty().SetFontSize(10)
        self.axes_actor.GetXAxisCaptionActor2D().GetCaptionTextProperty().SetColor(1, 0, 0)

        self.axes_actor.GetYAxisCaptionActor2D().GetTextActor().SetTextScaleModeToNone()
        self.axes_actor.GetYAxisCaptionActor2D().GetCaptionTextProperty().SetFontSize(10)
        self.axes_actor.GetYAxisCaptionActor2D().GetCaptionTextProperty().SetColor(0, 1, 0)

        self.axes_actor.GetZAxisCaptionActor2D().GetTextActor().SetTextScaleModeToNone()
        self.axes_actor.GetZAxisCaptionActor2D().GetCaptionTextProperty().SetFontSize(10)
        self.axes_actor.GetZAxisCaptionActor2D().GetCaptionTextProperty().SetColor(0, 0, 1)

        self.renderer.AddActor(self.axes_actor)

        # 设置渐变背景
        self.renderer.SetBackground(0.1, 0.2, 0.4)
        self.renderer.SetBackground2(1, 1, 1)
        self.renderer.SetGradientBackground(1)

        self.render_window.AddRenderer(self.renderer)

    def load_stl(self, stl_file: str):
        # 加载STL文件
        stl_reader = vtk.vtkSTLReader()
        stl_reader.SetFileName(stl_file)
        stl_reader.Update()  # 将数据加载到读取器中

        poly_data = stl_reader.GetOutput()  # 获取读取器输出数据

        # 映射器
        stl_mapper = vtk.vtkPolyDataMapper()
        if vtk.VTK_MAJOR_VERSION <= 5:
            stl_mapper.SetInput(poly_data)
        else:
            stl_mapper.SetInputData(poly_data)

        # 映射动作
        self.stl_actor.SetMapper(stl_mapper)

        # 映射场景
        self.renderer.AddActor(self.stl_actor)

        # 渲染窗口
        self.render_window.AddRenderer(self.renderer)
        self.render_window.Render()

    def load_csv(self, csv_file: str, column=3):
        # 文件读入
        data_frame = pd.read_csv(csv_file)
        data = data_frame.values.tolist()

        column_name = data_frame.columns.tolist()

        min_scalar = data_frame.iloc[:, column].min()
        max_scalar = data_frame.iloc[:, column].max()

        # 几何数据
        points = vtk.vtkPoints()
        for i in range(len(data)):
            points.InsertNextPoint(data[i][0], data[i][1], data[i][2])

        # 拓扑数据
        poly_vertex = vtk.vtkPolyVertex()
        poly_vertex.GetPointIds().SetNumberOfIds(len(data))
        for i in range(len(data)):
            poly_vertex.GetPointIds().SetId(i, i)

        # 属性数据
        points_scalars = vtk.vtkDoubleArray()
        for i in range(len(data)):
            points_scalars.InsertTuple1(i, data[i][column])

        # 颜色表
        lut = vtk.vtkLookupTable()
        lut.SetNumberOfTableValues(len(data))
        lut.SetNumberOfColors(256)
        lut.SetHueRange(0.7, 0)
        lut.SetAlphaRange(1.0, 1.0)
        lut.SetValueRange(1.0, 1.0)
        lut.SetSaturationRange(0.8, 0.8)
        lut.SetRange(min_scalar, max_scalar)
        lut.Build()

        # Create unstructured grid
        grid = vtk.vtkUnstructuredGrid()
        grid.Allocate(1, 1)
        grid.SetPoints(points)
        grid.InsertNextCell(poly_vertex.GetCellType(), poly_vertex.GetPointIds())
        grid.GetPointData().SetScalars(points_scalars)

        # 映射器
        points_mapper = vtk.vtkDataSetMapper()
        points_mapper.SetInputData(grid)
        points_mapper.ScalarVisibilityOn()
        points_mapper.SetScalarRange(min_scalar, max_scalar)
        points_mapper.SetLookupTable(lut)
        points_mapper.SetColorModeToDefault()

        # 创建标量栏
        self.scalar_bar_actor.GetTitleTextProperty().SetColor(0.1, 0.1, 0.1)
        self.scalar_bar_actor.GetTitleTextProperty().SetFontSize(5)
        self.scalar_bar_actor.GetTitleTextProperty().ItalicOff()
        self.scalar_bar_actor.SetLookupTable(points_mapper.GetLookupTable())
        self.scalar_bar_actor.SetTitle(column_name[3])
        self.scalar_bar_actor.SetNumberOfLabels(5)
        self.scalar_bar_actor.SetHeight(0.70)
        self.scalar_bar_actor.SetWidth(0.10)

        # 映射动作
        self.points_actor.SetMapper(points_mapper)
        self.points_actor.GetProperty().SetRepresentationToPoints()
        self.points_actor.GetProperty().SetPointSize(3)  # point_size_=3

        # 映射场景
        self.renderer.AddActor(self.points_actor)
        self.renderer.AddActor2D(self.scalar_bar_actor)

        # 渲染窗口
        self.render_window.AddRenderer(self.renderer)
        self.render_window.Render()

    def show(self):
        # 交互器
        self.render_window_interactor.SetRenderWindow(self.render_window)
        self.render_window_interactor.Start()


if __name__ == '__main__':
    visual = Visual()
    visual.load_stl("D:\\Project\\Visualization\\Data\\pocket-Ti.stl")
    visual.load_csv("D:\\Project\\Visualization\\Data\\kaicaototal.csv")
    visual.show()

