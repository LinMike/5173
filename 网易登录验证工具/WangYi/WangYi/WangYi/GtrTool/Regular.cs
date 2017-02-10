
namespace BaseCom
{

	using System;
	using System.Text.RegularExpressions;

	public static class Regular
	{

		public static Match SearchMatch(string Pattern, string SearchString, int MatchIndex)
		{
			MatchCollection matchs = SearchMatchs(Pattern, SearchString);

			if (matchs == null)
			{

				return null;
			}

			if (MatchIndex > (matchs.Count - 1))
			{

				return null;
			}
			Match match = matchs[MatchIndex];
			matchs = null;

			return match;
		}

		public static MatchCollection SearchMatchs(string Pattern, string SearchString)
		{
			MatchCollection matchs = new Regex(Pattern, RegexOptions.IgnoreCase).Matches(SearchString);

			if (matchs.Count <= 0)
			{

				return null;
			}

			return matchs;
		}

		public static object[] SearchMatchsTxts(string Pattern, string SearchString)
		{
			MatchCollection matchs = SearchMatchs(Pattern, SearchString);

			if (matchs == null)
			{

				return null;
			}
			object[] objArray = new object[matchs.Count];
			int index = 0;

			foreach (Match match in matchs)
			{
				int num2 = 0;
				string[] strArray = new string[match.Groups.Count];

				foreach (Group group in match.Groups)
				{
					strArray[num2] = group.Value;
					num2++;
				}
				objArray[index] = strArray;
				index++;
			}
			matchs = null;

			return objArray;
		}

		public static string[] SearchMatchTxts(string Pattern, string SearchString)
		{

			return SearchMatchTxts(Pattern, SearchString, 0);
		}

		public static string[] SearchMatchTxts(string Pattern, string SearchString, int MatchIndex)
		{
			Match match = SearchMatch(Pattern, SearchString, MatchIndex);

			if (match == null)
			{

				return null;
			}
			int index = 0;
			string[] strArray = new string[match.Groups.Count];

			foreach (Group group in match.Groups)
			{
				strArray[index] = group.Value;
				index++;
			}

			return strArray;
		}

		public static string SearchTxt(string Pattern, string SearchString)
		{

			return SearchTxt(Pattern, SearchString, 0, 1);
		}

		public static string SearchTxt(string Pattern, string SearchString, int MatchIndex)
		{

			return SearchTxt(Pattern, SearchString, MatchIndex, 1);
		}

		public static string SearchTxt(string Pattern, string SearchString, int MatchIndex, int Index)
		{
			string[] strArray = SearchMatchTxts(Pattern, SearchString, MatchIndex);

			if (strArray == null)
			{

				return "";
			}

			if (Index > strArray.Length)
			{

				return "";
			}
			string str = strArray[Index];
			strArray = null;

			return str;
		}
	}
}
