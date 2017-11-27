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
            ManipulabilityVoxel[] result;
            float[] maxB;
            float[] minB;
            float maxManip;
            using (var wrapper = new VirtualRobotManipulability())
            {
                wrapper.Init(0, null, Path.GetFullPath(@"Data\Robots\Puma560\Puma560.xml"), "robotNodeSet", "root", "tcp");

                result = wrapper.GetManipulability(50f, 0.2f, 10000, false, false, true, 50f);

                minB = wrapper.MinBox;
                maxB = wrapper.MaxBox;
                maxManip = wrapper.MaxManipulability;
            }

            if (result == null)
                return;

            for (var i = 0; i < Math.Min(10, result.Length); i++)
            {
                var entry = result[i];
                Console.WriteLine(@"[{0}, {1}, {2}, {3}, {4}, {5}] = {6}", entry.x, entry.y, entry.z, entry.a, entry.b, entry.c, entry.value);
            }

            Console.WriteLine(@"Results: {0}", result.Length);
            Console.WriteLine(@"Min: {0}, {1}, {2}, {3}, {4}, {5}", minB[0], minB[1], minB[2], minB[3], minB[4], minB[5]);
            Console.WriteLine(@"Max: {0}, {1}, {2}, {3}, {4}, {5}", maxB[0], maxB[1], maxB[2], maxB[3], maxB[4], maxB[5]);
            Console.WriteLine(@"Max. manipulability: {0}", maxManip);
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