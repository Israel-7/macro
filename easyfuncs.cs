using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ConsoleApp3
{
    class Program
    {
		[DllImport("user32.dll")]
		public static extern bool GetCursorPos(out Point pt);
		[DllImport("user32.dll")]
		public static extern bool SetCursorPos(int X, int Y);

		private static bool IsPointWihtinRect(Point pt, Rectangle rect)
		{
			return pt.X >= rect.Left && pt.Y >= rect.Top && pt.X <= rect.Right && pt.Y <= rect.Bottom;
		}

		public static Bitmap GetScreenshotFromRect(Rectangle rectangle)
		{
			Point pt;
			GetCursorPos(out pt);

			if (IsPointWihtinRect(pt, rectangle))
			{
				int x = rectangle.X - 100;
				int y = rectangle.Y;

				if (x < 0)
				{
					x = rectangle.Right + 100;
				}

				if (y < 0)
				{
					y = rectangle.Bottom / 2;
				}
				
				SetCursorPos(x, y);
			}

			Bitmap bitmap = new Bitmap(rectangle.Width, rectangle.Height, PixelFormat.Format24bppRgb);
			using (Graphics graphics = Graphics.FromImage(bitmap))
			{
				graphics.CopyFromScreen(rectangle.Left, rectangle.Top, 0, 0, bitmap.Size, CopyPixelOperation.SourceCopy);
			}

			SetCursorPos(pt.X, pt.Y);
			return bitmap;
		}

		private static readonly object _lockObject = new object();

		public static Rectangle SearchBitmap(Bitmap smallBmp, Bitmap bigBmp, int similarity = 100)
		{
			int width = bigBmp.Width;
			int mHeight = bigBmp.Height - smallBmp.Height + 1;
			int mWidth = smallBmp.Width * 3;
			int height = smallBmp.Height;

			Rectangle empty = Rectangle.Empty;

			lock (_lockObject)
			{
				BitmapData bitmapData = smallBmp.LockBits(new Rectangle(0, 0, smallBmp.Width, smallBmp.Height), ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
				BitmapData bitmapData2 = bigBmp.LockBits(new Rectangle(0, 0, bigBmp.Width, bigBmp.Height), ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);

				int stride = bitmapData.Stride;
				int stride2 = bitmapData2.Stride;
				byte[] array = new byte[bitmapData2.Stride * bitmapData2.Height];
				byte[] array2 = new byte[bitmapData.Stride * bitmapData.Height];

				Marshal.Copy(bitmapData2.Scan0, array, 0, bitmapData2.Stride * bitmapData2.Height);
				Marshal.Copy(bitmapData.Scan0, array2, 0, bitmapData.Stride * bitmapData.Height);

				int countera = 0;
				int width2 = smallBmp.Width;
				int value = stride2 - bigBmp.Width * 3;
				bool flag2 = true;

				for (int i = 0; i < mHeight; i++)
				{
					Application.DoEvents();

					for (int j = 0; j < width; j++)
					{
						int temp = countera;
						int counterb = 0;
						flag2 = true;

						for (int k = 0; k < height; k++)
						{
							for (int l = 0; l < mWidth; l++)
							{
								int num7 = (int)(array2[counterb] - array[countera]);
								int num8 = 1 + similarity;

								if (num7 > num8 || num7 < -num8)
								{
									flag2 = false;
									break;
								}

								countera++;
								counterb++;
							}

							if (!flag2) break;

							counterb = stride * (1 + k);
							countera = temp + stride2 * (1 + k);
						}

						if (flag2)
						{
							empty.X = j;
							empty.Y = i;
							empty.Width = smallBmp.Width;
							empty.Height = smallBmp.Height;

							break;
						}

						countera = temp;
						countera += 3;
					}

					if (flag2) break;
					countera += value;
				}

				bigBmp.UnlockBits(bitmapData2);
				smallBmp.UnlockBits(bitmapData);
			}

			return empty;
		}

		static int Main(string[] args)
		{
			if (args[0] == "FindImage")
			{
				Rectangle rectangle;

				if (args.Length == 7) {
					rectangle = new Rectangle(new Point(Convert.ToInt32(args[3]), Convert.ToInt32(args[4])),
						new Size(Convert.ToInt32(args[5]), Convert.ToInt32(args[6])));
				} else {
					rectangle = new Rectangle(0, 0, Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height);
				}

				Rectangle rect = SearchBitmap(new Bitmap(Image.FromFile(args[1])), GetScreenshotFromRect(rectangle));

				if (rect != Rectangle.Empty)
				{
					if (args[2] == "1") {
						SetCursorPos(rect.X + rect.Width / 2 + rectangle.X, rect.Y + rect.Height / 2 + rectangle.Y);
					}

					return 1;
				}
			}

			return 0;
		}
    }
}
