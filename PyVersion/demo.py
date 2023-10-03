#!/usr/bin/env python  
# noinspection PyUnresolvedReferences  
import vtkmodules.vtkInteractionStyle  
# noinspection PyUnresolvedReferences  
import vtkmodules.vtkRenderingOpenGL2  
from vtkmodules.vtkCommonColor import vtkNamedColors  
from vtkmodules.vtkFiltersSources import vtkConeSource  
from vtkmodules.vtkRenderingCore import (  
    vtkActor,  
    vtkPolyDataMapper,  
    vtkRenderWindow,  
    vtkRenderer  
)  
def main(argv):  
    #  
    # ����vtkNamedColorsʵ��������ѡ�����ͱ�������ɫ
    #  
    colors = vtkNamedColors()  
  
    #  
    # ����vtkConeSourceʵ�����趨һЩ���ԣ�
    # ��sourceʵ��ͨ�����ز���������׶ģ�����ݡ�        
    # VtkConeSourceʵ����cone���ǿ��ӻ�pipeline��һ���֡�
    # ������һЩ����filterҲ��ᴦ������ݣ����vtkPolyData�������ݣ���
    #  
    cone = vtkConeSource()  
    cone.SetHeight(3.0)  
    cone.SetRadius(1.0)  
    cone.SetResolution(10)  
    #  
    # ��ʾ���У�����ʹ��mapper���̶���������pipeline��
    # (�м��filter����vtkShrinkPolyDataҲ��ᱻ���뵽source��mapper֮��)
    # ���Ǵ���һ��ʵ������vtkPolyDataMapperȥ�����������ӳ�䵽����ͼ�β��С�
    # ���ǽ�cone source���ӵ����mapper������
    #  
    coneMapper = vtkPolyDataMapper()  #vtkPolyDataMapper����������ݱ���vtkPolyDataӳ�䵽����ͼ�β��С�
    coneMapper.SetInputConnection(cone.GetOutputPort())  #SetInputConnectionΪ����������˿������������ӣ�����������һ��filter������˿ڣ�ͨ��GetOutputPort��á�
    #  
    # ����һ��actor����cone��actor�߻�mapper����ͼ�β����Ⱦ��
    # һ��actorҲͨ��vtkPropertyʵ�������ı����ԣ���������һ���ڲ��任���������趨֮ǰ���������actor��mapperΪ��coneMapper����
    #  
    coneActor = vtkActor()  # ������һ����Ⱦ�����е�һ�����󣨼���ͼ�μ����ԣ���vtkActor���ڱ�ʾ��Ⱦ�����е�ʵ�壬���̳���vtkProp��actorsλ����غ�����
    coneActor.SetMapper(coneMapper)  # ����һ��actor��һ�����ӻ�pipeline�Ľ�β������mapper��
    coneActor.GetProperty().SetColor(colors.GetColor3d('MistyRose')) 
    #  
    # ����һ��Renderer����actors�����ȥ��һ��renderer����һ���Ӵ�������Ļ�е�һ�����߲��ִ��ڣ����ڻ�����ӵ�е�actors������Ҳ�������趨����ɫ��
    #  
    ren1 = vtkRenderer()  
    ren1.AddActor(coneActor)  
    ren1.SetBackground(colors.GetColor3d('MidnightBlue'))  
  
    # �������Ǵ���render���ڣ�������ʾ����Ļ�С�����ʹ��AddRenderer��renderer����render�����С�����Ҳ���ߴ��趨��300*300���ش�С��
    #  
    renWin = vtkRenderWindow()  
    renWin.AddRenderer(ren1)  
    renWin.SetSize(300, 300)  
    renWin.SetWindowName('Tutorial_Step1')  
  
    #  
    # ��������ѭ��360�㣬����ÿһ����Ⱦcone��
    #  
    for i in range(0, 360):  
        # ��Ⱦͼ��  
        renWin.Render()  
        # ��ת���1�㣬ע�⣺Azimuth����ת����Ĳ�����
        # ����ʹ��������Խ���Ϊ���ģ�view up���������нǶ���ת����ģ�Ͳ�����
        # ����view up��������Ϊ�趨�ģ�����������ϵ�µ��������ϵ��Y�᷽���ڹٷ��ĵ���vtkcamera���еĺ�������˵������
        # �����ⲿ����Ҳ����ں���������ֽ��н��ܣ��������Ҫ�Ļ�����  
        ren1.GetActiveCamera().Azimuth(1)  
if __name__ == '__main__':  
    import sys  
  
    main(sys.argv)  