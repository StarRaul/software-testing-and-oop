using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;
using OpenQA.Selenium.DevTools.V144.Audits;
using G2A_SearchTest.PageObjects;

namespace G2A_SearchTest
{
    [TestClass]
    public class Search
    {
        IWebDriver driver;
        SearchBar searchBar;

        [TestInitialize]

        public void Setup()
        {
            driver = new ChromeDriver();
            searchBar = new SearchBar(driver);
            driver.Navigate().GoToUrl("https://www.g2a.com/");
        }

        [TestCleanup]
        public void Cleanup()
        {
            driver.Quit();
        }
        [TestMethod]

        public void Search_withOneKeyword()
        {
            searchBar.SearchInput("steam");
            Thread.Sleep(2000);
            Assert.IsTrue(driver.Url.Contains("steam"));
        }

        [TestMethod]
        public void Search_andRefreshPage()
        {
            searchBar.SearchInput("steam");
            driver.Navigate().Refresh();
            Thread.Sleep(2000);
            Assert.IsTrue(driver.Url.Contains("steam"));
        }
    }
}