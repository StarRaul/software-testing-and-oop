using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using OpenQA.Selenium;
using OpenQA.Selenium.Chrome;
using CentricTester.PageObjects;
using OpenQA.Selenium.DevTools.V144.Audits;

namespace CentricTester
{
    [TestClass]
    public class LoginTests
    {
        IWebDriver driver;
        LoginPage loginPage;
        InventoryPage inventoryPage;

        [TestInitialize]
        public void Setup()
        {
            driver = new ChromeDriver();
            loginPage = new LoginPage(driver);
            inventoryPage = new InventoryPage(driver);
            driver.Navigate().GoToUrl("https://www.saucedemo.com/");
        }
        [TestCleanup]
        public void Cleanup()
        {
            driver.Quit();
        }
        [TestMethod]
        public void CheckLogin_WithValidCredentials()
        {
            /*
            IWebDriver driver = new ChromeDriver();
            driver.Navigate().GoToUrl("https://www.saucedemo.com/");
            var usernameTxt = driver.FindElement(By.Id("user-name"));
            usernameTxt.SendKeys("standard_user");
            var passwordTxt = driver.FindElement(By.Id("password"));
            passwordTxt.SendKeys("secret_sauce");
            var loginBtn = driver.FindElement(By.Id("login-button"));
            loginBtn.Click();
            */
            loginPage.Login("standard_user", "secret_sauce");
            var currentUrl = driver.Url;
            //var inventoryItems = driver.FindElements(By.CssSelector(".inventory_item"));
            Assert.AreEqual("https://www.saucedemo.com/inventory.html", currentUrl);
            Assert.AreEqual(6, inventoryPage.GetInventoryItemsCount()); //inventoryItems.Count
            driver.Quit();
        }
        [TestMethod]
        public void CheckLogin_WithInvalidCredentials() 
        {
            /*
            IWebDriver driver = new ChromeDriver();
            driver.Navigate().GoToUrl("https://www.saucedemo.com/");
            var usernameTxt = driver.FindElement(By.Id("user-name"));
            usernameTxt.SendKeys("standard_user");
            var passwordTxt = driver.FindElement(By.Id("password"));
            passwordTxt.SendKeys("pisica");
            var loginBtn = driver.FindElement(By.Id("login-button"));
            loginBtn.Click();
            */
            loginPage.Login("standard_user", "pisica");
            //var messageError = driver.FindElement(By.XPath("//h3[@data-test='error']"));
            Assert.AreEqual("Epic sadface: Username and password do not match any user in this service", loginPage.getErrorMessage()); //messageError.Text
            driver.Quit();
        }
        [TestMethod]
        public void CheckLogin_WithLockedOutCredentials()
        {
            /*
            IWebDriver driver = new ChromeDriver();
            driver.Navigate().GoToUrl("https://www.saucedemo.com/");
            var usernameTxt = driver.FindElement(By.Id("user-name"));
            usernameTxt.SendKeys("locked_out_user");
            var passwordTxt = driver.FindElement(By.Id("password"));
            passwordTxt.SendKeys("secret_sauce");
            var loginBtn = driver.FindElement(By.Id("login-button"));
            loginBtn.Click();
            */
            loginPage.Login("locked_out_user", "secret_sauce");
            //var messageError = driver.FindElement(By.XPath("//h3[@data-test='error']"));
            Assert.AreEqual("Epic sadface: Sorry, this user has been locked out.", loginPage.getErrorMessage());
            driver.Quit();
        }
    }
}
