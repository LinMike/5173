using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace GDI
{
    class Bases
    {

        /// <summary>
        /// 正则搜索 返回 指定Match索引 0  string 索引 1 的 string
        /// </summary>
        /// <param name="Pattern">规则</param>
        /// <param name="SearchString">要搜索的文本</param>
        /// 示例：Regular.SearchTxt("url = \"(.*?)\"", result)  不包括url 1
        /// <returns>string</returns>
        public static string SearchTxt(string Pattern, string SearchString)
        {
            return SearchTxt(Pattern, SearchString, 0, 1);
        }



        /// <summary>
        /// 正则搜索 返回 指定索引 1 的 string 2
        /// </summary>
        /// <param name="Pattern">规则</param>
        /// <param name="SearchString">要搜索的文本</param>
        /// <param name="MatchIndex">指定返回的索引Match</param>
        /// <returns>string</returns>
        public static string SearchTxt(string Pattern, string SearchString, int MatchIndex)
        {

            return SearchTxt(Pattern, SearchString, MatchIndex, 1);
        }

        /// <summary> 
        /// 正则搜索 返回 指定索引 string 3 大数组中小数组中的指字索引
        /// </summary>
        /// <param name="Pattern">规则</param>
        /// <param name="SearchString">要搜索的文本</param>
        /// <param name="MatchIndex">指定返回的索引Match</param>
        /// <param name="Index">指定索引</param>
        /// <returns>string</returns>
        public static string SearchTxt(string Pattern, string SearchString, int MatchIndex, int Index)
        {
            string[] Txts = SearchMatchTxts(Pattern, SearchString, MatchIndex);

            if (Txts == null) return "";

            if (Index > Txts.Length) return "";
            string Txt = Txts[Index];
            Txts = null;

            return Txt;
        }

        /// <summary>
        /// 正则搜索 返回 string[] 找到的集合是大数组中的小数组 
        /// </summary>
        /// <param name="Pattern">规则</param>
        /// <param name="SearchString">要搜索的文本</param>
        /// <param name="MatchIndex">指定返回的索引Match</param>
        /// 示例：Regular.SearchTxt("url = \"(.*?)\"", result)  包括url
        /// <returns>string[]</returns>
        public static string[] SearchMatchTxts(string Pattern, string SearchString, int MatchIndex)
        {
            Match Ma = SearchMatch(Pattern, SearchString, MatchIndex);

            if (Ma == null) return null;
            int i = 0;
            string[] Txts = new string[Ma.Groups.Count];

            foreach (Group Gp in Ma.Groups) { Txts[i] = Gp.Value; i++; }

            return Txts;
        }

        /// <summary>
        /// 正则搜索 返回 Match
        /// </summary>
        /// <param name="Pattern">规则</param>
        /// <param name="SearchString">要搜索的文本</param>
        /// <param name="MatchIndex">指定返回的索引Match</param>
        /// <returns>Match</returns>
        public static Match SearchMatch(string Pattern, string SearchString, int MatchIndex)
        {
            MatchCollection Mas = SearchMatchs(Pattern, SearchString);

            if (Mas == null) return null;

            if (MatchIndex > Mas.Count - 1) return null;
            Match Ma = Mas[MatchIndex];
            Mas = null;

            return Ma;
        }

        /// <summary>
        /// 正则搜索 返回 Match 集合
        /// </summary>
        /// <param name="Pattern">规则</param>
        /// <param name="SearchString">要搜索的文本</param>
        /// 例子：Object[] SearchMatchsTxts(string Pattern, string SearchString) 调用
        /// <returns>MatchCollection</returns>
        public static MatchCollection SearchMatchs(string Pattern, string SearchString)
        {
            Regex Reg = new Regex(Pattern, RegexOptions.IgnoreCase);
            MatchCollection Mas = Reg.Matches(SearchString);

            if (Mas.Count <= 0) return null;
            Reg = null;

            return Mas;
        }
    }
}
