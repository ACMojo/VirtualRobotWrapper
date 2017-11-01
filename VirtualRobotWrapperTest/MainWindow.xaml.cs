using System.IO;
using System.Windows;

using VirtualRobotWrapper;

namespace VirtualRobotWrapperTest
{
    /// <summary>
    ///     Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow
    {
        #region Instance

        public MainWindow()
        {
            InitializeComponent();
        }

        #endregion

        #region Private methods

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            using (var wrapper = new VirtualRobotManipulability())
            {
                wrapper.Run(Path.GetFullPath(@"C:\Users\alexa\source\repos\VirtualRobotWrapper\VirtualRobotWrapper\VirtualRobotWrapperTest\Data\Robots\ArmarIII\Puma560.xml"));
            }
        }

        #endregion
    }
}