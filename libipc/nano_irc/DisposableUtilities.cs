using System;
using System.Text;
using Windows.UI.Core;
using Windows.UI.Xaml.Controls;

namespace nano_irc
{
    class DisposableUtilities
    {
        public DisposableUtilities()
        {
            return;
        }
        // String manipulation tools.
        public String[] explode(String[] delimeters, String content)
        {
            return content.Split(delimeters, StringSplitOptions.RemoveEmptyEntries);
        }
        // Hashing and cryptocracy.
        /*  // #A7TODO :: port MD5 to universal
        public string GetHash()
        {
            Random rnd = new Random();
            int rnd_n = rnd.Next(Int32.MinValue, Int32.MaxValue);
            using (MD5 md5Hash = MD5.Create())
            {
                string hash = GetMD5Hash(md5Hash, rnd_n.ToString());
                //Console.WriteLine("The MD5 hash=" + hash + ".");
                return hash;
            }
        }
        private string GetMD5Hash(MD5 md5Hash, string input)
        {
            byte[] data = md5Hash.ComputeHash(Encoding.UTF8.GetBytes(input));
            StringBuilder sBuilder = new StringBuilder();
            for (int i = 0; i < data.Length; i++)
            {
                sBuilder.Append(data[i].ToString("x2"));
            }
            return sBuilder.ToString();
        }
        */
    }
}
