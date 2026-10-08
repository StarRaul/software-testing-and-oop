using Microsoft.VisualStudio.TestTools.UnitTesting;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;
using OpenQA.Selenium.DevTools.V144.Audits;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace G2A_SearchTest.PageObjects
{
    internal class SearchBar
    {
        IWebDriver driver;
        IWebElement SearchTxt => driver.FindElement(By.Id("search-input"));

        public SearchBar(IWebDriver driver)
        {
            this.driver = driver;
        }
        public void SearchInput(string input)
        {
            SearchTxt.SendKeys(input);
            SearchTxt.SendKeys(Keys.Enter);
        }
    }
}