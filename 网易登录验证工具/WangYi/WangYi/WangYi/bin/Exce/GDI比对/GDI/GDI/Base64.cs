using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GDI
{



    public sealed class Base64
    {
        public string EncodeBase64(string source)
        {
            if (!string.IsNullOrEmpty(source))
            {
                string a = source;
                byte[] b = System.Text.Encoding.Default.GetBytes(a);
                return Convert.ToBase64String(b);
            }
            return "";
        }

        public string DecodeBase64(string result)
        {
            if (!string.IsNullOrEmpty(result))
            {
                string a = result;
                byte[] c = Convert.FromBase64String(a);
                return System.Text.Encoding.Default.GetString(c);
            }
            return "";
        }
    }
}
