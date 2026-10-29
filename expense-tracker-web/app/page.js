

"use client";

import { useEffect, useState } from "react";

const categoryColors = {
  Travel: "#818cf8",
  Food: "#34d399",
  Shopping: "#fbbf24",
};

const money = (amount) =>
  new Intl.NumberFormat("en-IN", {
    style: "currency",
    currency: "INR",
    maximumFractionDigits: 2,
  }).format(amount);

export default function Home() {
  const [expenses, setExpenses] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState("");

  const [form, setForm] = useState({
  amount: "",
  category: "Food",
  date: "",
  description: "",
});

const [saving, setSaving] = useState(false);
const [formMessage, setFormMessage] = useState("");

  useEffect(() => {
    fetch("http://localhost:8080/api/expenses")
      .then((response) => {
        if (!response.ok) {
          throw new Error("Could not load expenses");
        }
        return response.json();
      })
      .then((data) => {
        setExpenses(data);
      })
      .catch((err) => {
        console.error("API error:", err);
        setError("Unable to load expenses. Check whether your C++ API is running.");
      })
      .finally(() => {
        setLoading(false);
      });

      setForm((currentForm) => ({
  ...currentForm,
  date: new Date().toISOString().slice(0, 10),
}));
  }, []);

  const categories = Object.values(
    expenses.reduce((result, expense) => {
      const name = expense.category.trim();

      if (!result[name]) {
        result[name] = {
          name,
          amount: 0,
          color: categoryColors[name] || "#94a3b8",
        };
      }

      result[name].amount += Number(expense.amount);
      return result;
    }, {})
  );

  const total = expenses.reduce(
    (sum, expense) => sum + Number(expense.amount),
    0
  );

  const topCategory = [...categories].sort(
    (a, b) => b.amount - a.amount
  )[0];

async function handleAddExpense(event) {
  event.preventDefault();
  setSaving(true);
  setFormMessage("");

  try {
    const body = new URLSearchParams({
      amount: form.amount,
      category: form.category,
      date: form.date,
      description: form.description,
    });

    const response = await fetch("http://localhost:8080/api/expenses", {
      method: "POST",
      headers: {
        "Content-Type": "application/x-www-form-urlencoded",
      },
      body: body.toString(),
    });

    if (!response.ok) {
      throw new Error(await response.text());
    }

    setFormMessage("Expense added successfully!");

    setForm({
      amount: "",
      category: "Food",
      date: new Date().toISOString().slice(0, 10),
      description: "",
    });

    const updatedResponse = await fetch(
      "http://localhost:8080/api/expenses"
    );
    const updatedExpenses = await updatedResponse.json();
    setExpenses(updatedExpenses);
  } catch (error) {
    console.error(error);
    setFormMessage("Could not add expense. Check the C++ API.");
  } finally {
    setSaving(false);
  }
}
  return (
        <>
      {loading && <p>Loading expenses...</p>}
      {error && <p>{error}</p>}
      {!loading && !error && (
    <main className="dashboard">
      <aside className="sidebar">
        <div className="brand">
          <div className="brand-icon">₹</div>
          <span>Expense<span className="brand-light">Flow</span></span>
        </div>

        <p className="nav-label">WORKSPACE</p>
        <nav>
          <a className="nav-item active" href="#overview">▦ <span>Overview</span></a>
          <a className="nav-item" href="#transactions">↔ <span>Transactions</span></a>
          <a className="nav-item" href="#categories">◷ <span>Categories</span></a>
        </nav>

        <div className="sidebar-bottom">
          <div className="profile-avatar">ET</div>
          <div>
            <strong>Expense Tracker</strong>
            <p>Personal finances</p>
          </div>
        </div>
      </aside>

      <section className="main-content" id="overview">
        <header className="topbar">
          <div>
            <p className="eyebrow">YOUR FINANCES AT A GLANCE</p>
            <h1>Dashboard</h1>
            <p className="subtitle">Welcome back! Here is your spending overview.</p>
          </div>
          <div className="date-chip">◷ &nbsp; October 2026</div>
        </header>
         <section className="panel" style={{ marginBottom: "24px" }}>
  <h3>Add a new expense</h3>
  <p className="muted">Record an expense directly from your dashboard.</p>

  <form
    onSubmit={handleAddExpense}
    style={{
      display: "grid",
      gridTemplateColumns: "repeat(auto-fit, minmax(180px, 1fr))",
      gap: "16px",
      marginTop: "20px",
    }}
  >
    <label>
      Amount (₹)
      <input
        type="number"
        min="0.01"
        step="0.01"
        required
        value={form.amount}
        onChange={(event) =>
          setForm({ ...form, amount: event.target.value })
        }
      />
    </label>

    <label>
      Category
      <select
        value={form.category}
        onChange={(event) =>
          setForm({ ...form, category: event.target.value })
        }
      >
        <option>Food</option>
        <option>Travel</option>
        <option>Shopping</option>
        <option>Education</option>
        <option>Health</option>
        <option>Other</option>
      </select>
    </label>

    <label>
      Date
      <input
        type="date"
        required
        value={form.date}
        onChange={(event) =>
          setForm({ ...form, date: event.target.value })
        }
      />
    </label>

    <label>
      Description
      <input
        type="text"
        required
        maxLength={100}
        value={form.description}
        onChange={(event) =>
          setForm({ ...form, description: event.target.value })
        }
        placeholder="e.g. Lunch"
      />
    </label>

    <button type="submit" disabled={saving}>
      {saving ? "Saving..." : "Add expense"}
    </button>
  </form>

  
{formMessage && (
  <p
    className={`form-message ${
      formMessage === "Expense added successfully!"
        ? "form-message-success"
        : "form-message-error"
    }`}
    role="status"
    aria-live="polite"
  >
    {formMessage === "Expense added successfully!" && "✓ "}
    {formMessage !== "Expense added successfully!" && "⚠ "}
    {formMessage}
  </p>
)}

</section>
        <section className="stats-grid">
          <article className="stat-card">
            <div className="stat-heading">
              <span>Total spending</span><span className="stat-icon purple">₹</span>
            </div>
            <h2>{money(total)}</h2>
            <p className="muted">Across all transactions</p>
          </article>

          <article className="stat-card">
            <div className="stat-heading">
              <span>Transactions</span><span className="stat-icon green">↔</span>
            </div>
            <h2>{expenses.length}</h2>
            <p className="muted">Recorded expenses</p>
          </article>

          <article className="stat-card">
            <div className="stat-heading">
              <span>Top category</span><span className="stat-icon yellow">✦</span>
            </div>
            <h2>{topCategory?.name || "None"}</h2>
          <p className="muted">
           {total > 0
            ? `${((topCategory.amount / total) * 100).toFixed(2)}% of total spending`
         : "No expenses recorded"}
        </p>
          </article>
        </section>

        <section className="content-grid">
          <article className="panel" id="categories">
            <div className="panel-heading">
              <div>
                <h3>Spending by category</h3>
                <p className="muted">How your money is distributed</p>
              </div>
              <span className="period-tag">This month</span>
            </div>

            <div className="category-list">
              {categories.map((category) => {
                const percentage = total > 0
                ? (category.amount / total) * 100
                   : 0;
                return (
                  <div className="category-row" key={category.name}>
                    <div className="category-details">
                      <span className="category-dot" style={{ background: category.color }} />
                      <span>{category.name}</span>
                      <strong>{money(category.amount)}</strong>
                    </div>
                    <div className="progress-track">
                      <div
                        className="progress-fill"
                        style={{ width: `${percentage}%`, background: category.color }}
                      />
                    </div>
                    <p className="percentage">{percentage.toFixed(2)}% of total</p>
                  </div>
                );
              })}
            </div>

            <div className="total-row">
              <span>Total spending</span>
              <strong>{money(total)}</strong>
            </div>
          </article>

          <article className="panel quick-panel">
            <h3>Expense summary</h3>
            <p className="muted">Your current overview</p>
            <div className="summary-item">
              <span className="summary-symbol">₹</span>
              <div><p>Total recorded</p><strong>{money(total)}</strong></div>
            </div>
            <div className="summary-item">
              <span className="summary-symbol">#</span>
              <div><p>Categories used</p><strong>{categories.length}</strong></div>
            </div>
            <div className="summary-item">
              <span className="summary-symbol">↗</span>
              <div><p>Highest category</p><strong>{topCategory?.name || "None"}</strong></div>
            </div>
           <p className="note">Live expense data from your C++ backend.</p>
          </article>
        </section>

        <section className="panel transactions-panel" id="transactions">
          <div className="panel-heading">
            <div>
              <h3>Recent transactions</h3>
              <p className="muted">A quick look at your recorded expenses</p>
            </div>
            <span className="period-tag">{expenses.length} records</span>
          </div>

          <div className="table-wrap">
            <table>
              <thead>
                <tr><th>DESCRIPTION</th><th>CATEGORY</th><th>DATE</th><th className="amount-cell">AMOUNT</th></tr>
              </thead>
              <tbody>
                {expenses.map((expense) => (
                  <tr key={expense.id}>
                    <td><strong>{expense.title}</strong></td>
                    <td>
<span
  className={`category-pill category-${String(expense.category || "other")
    .trim()
    .toLowerCase()
    .replace(/\s+/g, "-")}`}
>
  {String(expense.category || "Other").trim()}
</span>
</td>
                    <td className="muted">{expense.date}</td>
                    <td className="amount-cell amount">{money(expense.amount)}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </section>

        <footer>ExpenseFlow · C++ Expense Tracker Project</footer>
      </section>
    </main>
        )}
    </>
  );
}

