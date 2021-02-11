class CreateEmails < ActiveRecord::Migration[5.1]
  def change
    create_table :emails do |t|
      t.string :email_address
      t.references :user, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
