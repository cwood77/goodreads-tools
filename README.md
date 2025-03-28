# goodreads-tools
Tools for maintaining goodreads data

On goodreads.com, your book data can be exported/imported as a comma-separated values (CSV) file.  This data includes all your books, the dates you read them, any reviews you made, and any tags you assigned the books.  The general idea is, you export your data, run _grtools_ on it, then import it back.  This allows grtools to perform housekeeping operations on your data.

# Export & Import
To Export your goodreads database to CSV
   - In a webbrowser, navigate to _www.goodreads.com_
   - On _My Books_, under Tools choose _Import and Export_
   - Under _Export_, choose _Export Library_
   - Click the hyperlink to download the export as a CSV file

Import works in a similar way.  Reimporting your existing books simply overwrites the existing data.

# Prepare
Some columns in the goodreads CSV format (like 'Bookshelves with positions') are not maintained by grtools.  Others, like 'Bookshelves' are unstable in that goodreads lists the contents in random order.

In order to work with the CSV, the _prepare_ verb first strips this unmaintained column, and sorts the contents of the 'Bookshelves' column.  This makes diffing the file later a lot more readable.

# Split & Merge
As I find the tagging feature in goodreads.com cumbersome, you can use split/merge to manipulate tags by dragging and dropping files in Windows File Explorer.

For example, "grtools --split C:\goodreads_library_export.csv C:\mybooks s-\*" will split a single CSV file into a bunch of tiny CSV files, with one book per file.  Each file is named the title of the book with the CSV extension.  The syntax "s-*" tells grtools to look for tags that begin with s- and create folders for those tags under "C:\mybooks"; each book's CSV will be placed under the tag folder it's marked with.  If a book belongs to multiple such tags, copies of the file will be created.  Tags that don't match the pattern are ignored.

## Merge

## Tag Syntax
