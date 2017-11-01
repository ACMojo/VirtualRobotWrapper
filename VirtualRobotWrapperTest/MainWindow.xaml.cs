using System;
using System.IO;
using System.Windows;

using VirtualRobotWrapper;

using MathGeoLibWrapper;

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

        private void ButtonVirtualRobot_Click(object sender, RoutedEventArgs e)
        {
            using (var wrapper = new VirtualRobotManipulability())
            {
                wrapper.Run(Path.GetFullPath(@"C:\Users\alexa\source\repos\VirtualRobotWrapper\VirtualRobotWrapper\VirtualRobotWrapperTest\Data\Robots\ArmarIII\Puma560.xml"));
            }
        }

        private void ButtonOBB_Click(object sender, RoutedEventArgs e)
        {
            var random = new Random(5);

            var points = new double[4][];
            for (var i = 0; i < points.Length; i++)
            {
                points[i] = new[] { random.NextDouble() * 100, random.NextDouble() * 100, random.NextDouble() * 100, 0 };
            }

            using (var wrapper = new OBBWrapper(points))
            {
                var position = wrapper.Position;
                var halfExtents = wrapper.HalfExtents;
                var axis = wrapper.Axis;

                Console.WriteLine($@"Position: {position[0]}, {position[1]}, {position[2]}");
                Console.WriteLine($@"Half extents: {halfExtents[0]}, {halfExtents[1]}, {halfExtents[2]}");
                Console.WriteLine($@"Axis 1: {axis[0][0]}, {axis[0][1]}, {axis[0][2]}");
                Console.WriteLine($@"Axis 2: {axis[0][0]}, {axis[0][1]}, {axis[0][2]}");
                Console.WriteLine($@"Axis 3: {axis[0][0]}, {axis[0][1]}, {axis[0][2]}");
            }
        }

        #endregion
    }
}