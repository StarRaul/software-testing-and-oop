using OpenQA.Selenium;
using System;
using System.Collections.Generic;
using System.Text;

namespace CentricTester.PageObjects
{
    internal class LoginPage
    {
        IWebDriver driver;
        IWebElement usernameTxt => driver.FindElement(By.Id("user-name"));
        IWebElement passwordTxt => driver.FindElement(By.Id("password"));
        IWebElement loginBtn => driver.FindElement(By.Id("login-button"));
        IWebElement messageError=> driver.FindElement(By.XPath("//h3[@data-test='error']"));

        public LoginPage(IWebDriver driver)
        {
            this.driver = driver;
        }
        public void Login(string username, string password)
        {
            usernameTxt.SendKeys(username);
            passwordTxt.SendKeys(password);
            loginBtn.Click();
        }
        public string getErrorMessage()
        {
            return messageError.Text;
        }
    }
}
