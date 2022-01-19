# QssCreator

QssCreator是一款对qss文件进行编辑的软件

取名由来:QssEditor已经有别的项目在用了,所以就参考QtCreator取了名字,软件图标也参照了QtCreator的图标

设计思路:刚开始我使用的是[hustlei/QssStylesheetEditor](https://github.com/hustlei/QssStylesheetEditor)来编辑qss,但是在实际开发中这块软件对于颜色变量的使用与我的想法有冲突,我的想法是把颜色变量独立出来放到一个独立的文件里作为主题文件,再程序运行时根据选择的主题来替换颜色变量,而不是一开始就导出为不包含变量的qss使用.本来想QssStylesheetEditor源代码上面改的,但是发现是PyQt写的,本人对于PyQt不太熟,且还有许多与QssStylesheetEditor不同的想法,故打算使用QWidget开发.

### 参考与学习

#### 思路

颜色定义的思路来自:[hustlei/QssStylesheetEditor](https://github.com/hustlei/QssStylesheetEditor)

#### 控件

查找和替换对话框:[Yet-Zio/QFindDialogs](https://github.com/Yet-Zio/QFindDialogs)

侧边的TabWidget修改自:[qt-creator](https://github.com/qt-creator/qt-creator)的FancyTabWidget

#### Ui设计

Ui是仿照QtCreator的主界面Ui

Dark主题的配色参考的是QtCreator的Flat Dark主题,文本编辑器的配色及高亮参考的是我最喜欢的Solarized Dark



### 一些说明

#### 1.颜色定义文件

文件后缀:qssdef

格式如下:

``` //Warning 
//1.变量命名要使用$开头
//2.变量不要使用_作为最后一个字符
//3.value结尾推荐使用 ';'
$side_bkg = #404142; //(64, 65, 66); 
$side_front = #bec0c1;//bec0c1; //190, 192, 193
$bkg = #2e2f30;// 46, 47, 48
$text=#d0d0d0;
$select_bkg=#1d545c;
$border_color=#000000;
$side_border=#323334;
$headViewSectionH_text=#323334;
$headViewSectionH_bkg=#bec0c1;
$scrollBar_page=#494a4b;
```

### 软件功能

#### 1.支持实时切换主题,无需重启程序

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/ThemeDark.png) ![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/ThemeNone.png)



#### 2.支持将颜色变量文件以TableView显示

1.支持右键添加新的颜色变量,移除已有变量

2.支持根据变量名或者颜色排序,打开排序后可点击表头修改排序规则,关闭排序按照文件原本顺序显示变量

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/ColorDefWidgetMenu.png)

3.支持修改变量名(双击变量名)

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/ChangeDefName.png)

4.支持使用QColorDialog修改颜色(双击颜色)

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/ChangeColor.png)

#### 3.支持文本高亮和自动补全(使用中文输入法时补全将不工作)

1.支持补全Qt关键词(参考QssCreator\conf\QtClassKeyWord.txt)

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/AutoCQtClass.png)

2.支持补全Qss关键词(参考QssCreator\conf\qssKeyWord.txt)

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/AutoCQssKeyword.png)

3.支持补全用户定义的颜色变量名称,能够随用户修改实时更新,输入$即可呼出补全列表

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/AutoCDef.png)



4.支持对以上三种内容高亮显示

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/Highlight.png) ![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/HighlightNoneTheme.png)

5.支持换行时补全tab,和}

#### 预览Qss效果

点击Menubar上的Preview按钮即可显示一个加载qss和qssdef的窗口

![](https://github.com/chenghongfeng/QssCreator/blob/master/READMEimages/PreviewWidget.png) 



### TODO 1.0 Version

完成后正式发布1.0版本

- [ ] 软件中文翻译

- [ ] README英文版本

- [ ] Linux/Mac版本

- [ ] 更新检查

- [ ] 替换颜色变量的算法优化

- [ ] Preview窗口优化和更多控件支持

- [ ] 所有控件的颜色更符合主题

- [ ] 支持修改和添加本软件主题

- [ ] 代码优化

- [ ] 一个示例程序

  

