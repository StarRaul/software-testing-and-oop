using OpenQA.Selenium;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;

namespace CentricTester.PageObjects
{
    internal class InventoryPage
    {
        IWebDriver driver;
        ReadOnlyCollection<IWebElement> inventoryItems => driver.FindElements(By.CssSelector(".inventory_item"));
        public InventoryPage(IWebDriver driver)
        {
            this.driver = driver;
        }
        public int GetInventoryItemsCount()
        {
            return inventoryItems.Count;
        }
    }
}
